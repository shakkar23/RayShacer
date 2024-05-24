// rendering
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "Rendering.hpp"
#include "../Utility/Color.hpp"
#include "../World/World.hpp"
#include "../Utility/vec2.hpp"


__device__ ShakColor render_pixel(const World& world, const vec2 coord, const player_info& player) {
	const auto [playerX,
		playerY,
		playerZ,
		playerYaw,
		playerPitch
	] = player;
	constexpr float FOV = 90.0f;

	// convert player angles to a looking vector
	// the 1/tan(fov/2) is the distance from the player to the screen
	float eyePos = (float)-1 / tan(toRadians(FOV / 2.0f));


	//vec3 rayDirection = (vec3(coord.x, coord.y, -eyePos)).normalize().rotateYP(playerYaw, playerPitch);
	vec3 rayDirection = (vec3(coord.x, coord.y, -eyePos))
		.normalize().rotateYP(playerYaw, playerPitch);

	ShakColor pixel;
	pixel.r = 0;
	pixel.g = 0;
	pixel.b = 0;
	pixel.a = 255;

	// color if it hits nothing
	vec3 normal = rayDirection * 0.5f + 0.5f;

	vec3 rayOrigin = vec3(
		playerX,
		playerY,
		playerZ
	);
	World::blockInfo block_info;
	constexpr int MAX_BOUNCES = 100;
	int i = 0;
	do {
		block_info = world.voxel_traversal_new(rayOrigin, rayDirection);
		float face{};
		vec3 trunc = block_info.where.fract();
		{
			// find face hit, its the one thats either 0.0001 or 0.999, aka the one thats closest to 0 or 1
			auto minval = std::min(std::min(trunc.x, trunc.y), trunc.z);
			auto maxval = std::max(std::max(trunc.x, trunc.y), trunc.z);
			// find which is closer, minval to 0, or maxval to 1
			// if minval is closer to 0, then the face is the minval
			// if maxval is closer to 1, then the face is the maxval
			auto normminval = minval - 0;
			auto normmaxval = (maxval - 1) * -1;
			face = normminval < normmaxval ? minval : maxval;
		}
		switch (block_info.type)
		{
		case blockType::Air:
			break;
		case blockType::Dirt:
		{
			if (face == trunc.x)
				pixel.r = trunc.z * 255;
			else if (face == trunc.y)
				pixel.g = trunc.x * 255;
			else
				pixel.b = trunc.y * 255;
			break;
		}
		case blockType::Stone:
			pixel.r = 100;
			pixel.g = 100;
			pixel.b = 100;
			break;

		case blockType::Mirror:
		{
			vec3 block_normal;

			// find the normal of the block

			if (face == trunc.x)
				block_normal = { 1,0,0 };
			else if (face == trunc.y)
				block_normal = { 0,1,0 };
			else
				block_normal = { 0,0,1 };

			if (block_info.where.x < rayOrigin.x ||
				block_info.where.y < rayOrigin.y ||
				block_info.where.z < rayOrigin.z)
				block_normal *= -1.0f;

			// reflect the ray
			rayDirection = rayDirection - (block_normal * 2.0f * rayDirection.dotThis(block_normal));
			normal = rayDirection * 0.5f + 0.5f;

			rayOrigin = block_info.where;
			break;

		}
		default:
			pixel.r = normal.x * 255;
			pixel.g = normal.y * 255;
			pixel.b = normal.z * 255;
			break;
		}

		++i;

		if (i >= MAX_BOUNCES) {
			pixel.r = 255;
			pixel.g = 255;
			pixel.b = 255;
		}
	} while (i < MAX_BOUNCES && block_info.type == Mirror);

	return pixel;
}

__global__ void render(World const * const world, const std::span<vec2> coords, std::span<ShakColor> colors, const player_info player) {
	int i = (blockIdx.x * blockDim.x) + threadIdx.x;

	if (i < colors.size()) {
		vec2 coord = { (float)i / 256.0f, 0.5f };

		ShakColor pixel = render_pixel(*world, coord, player);

		// write to the screen
		colors[i] = pixel;
	}
}

void wrapper_thing(const World& world, const std::span<vec2> coords, std::span<ShakColor> colors, player_info &info) {

	vec2* d_coords;
	ShakColor* d_colors;
	World *d_world;

	// sending data to the gpu
	cudaMalloc(&d_coords, coords.size_bytes());
	cudaMalloc(&d_colors, colors.size_bytes());
	cudaMalloc(&d_world, sizeof(World));

	cudaMemcpy(d_coords, coords.data(), coords.size_bytes(), cudaMemcpyHostToDevice);
	cudaMemcpy(d_colors, colors.data(), colors.size_bytes(), cudaMemcpyHostToDevice);
	cudaMemcpy(d_world,  &world,		sizeof(World),		 cudaMemcpyHostToDevice);
	cudaDeviceSynchronize();

	std::span<vec2> d_coords_s(d_coords, coords.size());
	std::span<ShakColor> d_colors_s(d_colors, colors.size());
	render <<<(colors.size() / 256 + 1), 256>>> (d_world, d_coords_s, d_colors_s, info);

	cudaDeviceSynchronize();

	// get the data back
	cudaMemcpy(colors.data(), d_colors, colors.size_bytes(), cudaMemcpyDeviceToHost);
	cudaMemcpy(coords.data(), d_coords, coords.size_bytes(), cudaMemcpyDeviceToHost);
	// we dont care about the world, it was not modified in the kernel

	cudaDeviceSynchronize();

	cudaFree(d_coords);
	cudaFree(d_colors);
	cudaFree(d_world);
}

