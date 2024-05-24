#pragma once
#include <array>

#include "../Utility/vec3.hpp"

#include <limits>
inline const vec3 renderDistance = vec3(100, 100, 100);
enum blockType {
	Air,
	Dirt,
	Stone,
	Mirror,
	OutOfBounds
};
constexpr int blockSize = 9;
constexpr int blockCount = 9;

class World {
public:
	std::array<std::array<std::array<blockType, blockCount>, blockCount>, blockCount> world = initWorld();

	constexpr inline std::array<std::array<std::array<blockType, blockCount>, blockCount>, blockCount> initWorld() {
		// fill the bottom most y row with dirt
		std::array<std::array<std::array<blockType, blockCount>, blockCount>, blockCount> tmp{};
		for (int x = 0; x < blockCount; x++) {
			for (int z = 0; z < blockCount; z++) {
				tmp[x][0][z] = Dirt;
			}
		}

		tmp[5][5][5] = Dirt;
		tmp[6][6][6] = Dirt;
		tmp[4][4][4] = Mirror;
		tmp[4][4][6] = Mirror;
		tmp[2][2][2] = Stone;

		return tmp;
	}

	struct blockInfo {
		vec3 where;
		blockType type;
	};

	constexpr inline blockInfo voxel_traversal_old(const vec3 ray_start, const vec3 ray_direction) const {
		//auto voxelSize = 1;


		// This id of the first/current voxel hit by the ray.
		// Using floor (round down) is actually very important,
		// the implicit int-casting will round up for negative numbers.
		vec3 current_voxel(ray_start.floor());

		// Compute normalized ray direction.

		// In which direction the voxel ids are incremented.
		const vec3 direction = vec3(
			(ray_direction.x >= 0.0f) ? 1 : -1,
			(ray_direction.y >= 0.0f) ? 1 : -1,
			(ray_direction.z >= 0.0f) ? 1 : -1);

		//const bool down = ray_direction.y < 0.0f;


		// Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).

		const vec3 next_voxel_boundary = current_voxel + direction;


		// tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
		// the value of t at which the ray crosses the first vertical voxel boundary
		vec3 tMax = vec3(
			(ray_direction.x != 0.0f) ? (next_voxel_boundary.x - ray_start.x) / ray_direction.x : DBL_MAX,
			(ray_direction.y != 0.0f) ? (next_voxel_boundary.y - ray_start.y) / ray_direction.y : DBL_MAX,
			(ray_direction.z != 0.0f) ? (next_voxel_boundary.z - ray_start.z) / ray_direction.z : DBL_MAX
		);

		// tDeltaX, tDeltaY, tDeltaZ --
		// how far along the ray we must move for the horizontal component to equal the width of a voxel
		// the direction in which we traverse the grid
		// can only be FLT_MAX if we never go in that direction
		vec3 t_delta = direction / ray_direction;

		/*vec3 fudge = (direction + 1.0f) / 2.0f;
		vec3 t_curr = t_delta * (fudge - (ray_start.fract()) * direction - 1.0);
		auto position = ray_start.floor().as_ivec3();
		auto step = direction.as_ivec3();
		float empty_size = 1.0;*/

		const auto getTile = [this](const vec3 current_voxel) {

			// if in bounds of the world
			if (current_voxel.x >= 0.0f && current_voxel.x < blockCount &&
				current_voxel.y >= 0.0f && current_voxel.y < blockCount &&
				current_voxel.z >= 0.0f && current_voxel.z < blockCount) {
				return this->world[(int)current_voxel.x][(int)current_voxel.y][(int)current_voxel.z];
			}
			else {
				return blockType::OutOfBounds;
			}
			};

		vec3 renderDistanceAccumalator = vec3(0.0f, 0.0f, 0.0f);
		blockType tile = getTile(current_voxel);
		float distance = 0;
		while (tile == blockType::Air) {

			// walk along shortest path
			if (tMax.x < tMax.y) {
				if (tMax.x < tMax.z) {
					current_voxel.x += direction.x;
					distance = tMax.x;
					tMax.x += t_delta.x;
					if (renderDistanceAccumalator.x > renderDistance.x) {
						break;
					}
					renderDistanceAccumalator.x += t_delta.x;
				}
				else {
					current_voxel.z += direction.z;
					distance = tMax.z;
					tMax.z += t_delta.z;
					if (renderDistanceAccumalator.z > renderDistance.z) {
						break;
					}
					renderDistanceAccumalator.z += t_delta.z;
				}
			}
			else {
				if (tMax.y < tMax.z) {
					current_voxel.y += direction.y;
					distance = tMax.y;
					tMax.y += t_delta.y;
					if (renderDistanceAccumalator.y > renderDistance.y) {
						break;
					}
					renderDistanceAccumalator.y += t_delta.y;
				}
				else {
					current_voxel.z += direction.z;
					distance = tMax.z;
					tMax.z += t_delta.z;
					if (renderDistanceAccumalator.z > renderDistance.z) {
						break;
					}
					renderDistanceAccumalator.z += t_delta.z;
				}
			}
			tile = getTile(current_voxel);
		}

		// calculate the position of the hit
		vec3 hit_position = ray_start + ray_direction * distance;

		return { hit_position , tile };
	}



	constexpr inline blockInfo voxel_traversal_new(const vec3 SourcePos, const vec3 Dir) const {

		// source: https://www.researchgate.net/publication/233899848_Efficient_implementation_of_the_3D-DDA_ray_traversal_algorithm_on_GPU_and_its_application_in_radiation_dose_calculation

		// 1 initialize ray information, source point S, and direction Dir
		// 2 initialize voxel information (world)
		// 3 initialize CurrentV as indicies of the voxel where S is;
		auto CurrentV = SourcePos.floor();

		const vec3 step = vec3(
			(Dir.x != 0.0f) ? ((Dir.x < 0.0f) ? -1 : 1) : std::numeric_limits<float>::max(),
			(Dir.y != 0.0f) ? ((Dir.y < 0.0f) ? -1 : 1) : std::numeric_limits<float>::max(),
			(Dir.z != 0.0f) ? ((Dir.z < 0.0f) ? -1 : 1) : std::numeric_limits<float>::max()
		);

		const auto invDir = 1.0f / Dir * step;
		// T = distance along the ray from SourcePos to the boundary of CurrentV
		//auto MaxT = (CurrentV + step) - SourcePos * invDir;

		auto RayLengthScaler = vec3(
			sqrt(
				1 * 1 +
				Dir.y / Dir.x * Dir.y / Dir.x +
				Dir.z / Dir.x * Dir.z / Dir.x
			),
			sqrt(
				1 * 1 +
				Dir.x / Dir.y * Dir.x / Dir.y +
				Dir.z / Dir.y * Dir.z / Dir.y
			),
			sqrt(
				1 * 1 +
				Dir.x / Dir.z * Dir.x / Dir.z +
				Dir.y / Dir.z * Dir.y / Dir.z
			)

		);
		auto MaxT = vec3(
			step.x == 1 ? ((CurrentV.x + 1) - SourcePos.x) * RayLengthScaler.x : (SourcePos.x - CurrentV.x) * RayLengthScaler.x,
			step.y == 1 ? ((CurrentV.y + 1) - SourcePos.y) * RayLengthScaler.y : (SourcePos.y - CurrentV.y) * RayLengthScaler.y,
			step.z == 1 ? ((CurrentV.z + 1) - SourcePos.z) * RayLengthScaler.z : (SourcePos.z - CurrentV.z) * RayLengthScaler.z
		);

		const auto VoxelSize = vec3(1, 1, 1);
		const auto DeltaT = VoxelSize * invDir;

		vec3 VoxelIncr = vec3(0, 0, 0);


		/*

			if (current_voxel[0]!=last_voxel[0] && ray[0]<0) { diff[0]--; neg_ray=true; }
			if (current_voxel[1]!=last_voxel[1] && ray[1]<0) { diff[1]--; neg_ray=true; }
			if (current_voxel[2]!=last_voxel[2] && ray[2]<0) { diff[2]--; neg_ray=true; }
			visited_voxels.push_back(current_voxel);
			if (neg_ray) {
			  current_voxel+=diff;
			}
		*/




		const auto getTile = [this](const vec3 current_voxel) {

			// if in bounds of the world
			if (current_voxel.x >= 0.0f && current_voxel.x < blockCount &&
				current_voxel.y >= 0.0f && current_voxel.y < blockCount &&
				current_voxel.z >= 0.0f && current_voxel.z < blockCount) {
				return this->world[(int)current_voxel.x][(int)current_voxel.y][(int)current_voxel.z];
			}
			else {
				return blockType::OutOfBounds;
			}
			};
		blockType tile = blockType::Air;
		while (tile == blockType::Air) {
			VoxelIncr.x = (MaxT.x <= MaxT.y) && MaxT.x <= MaxT.z;
			VoxelIncr.y = (MaxT.y <= MaxT.x) && MaxT.y <= MaxT.z && !VoxelIncr.x;
			VoxelIncr.z = (MaxT.z <= MaxT.x) && MaxT.z <= MaxT.y && !VoxelIncr.x && !VoxelIncr.y;

			MaxT += VoxelIncr * DeltaT;

			CurrentV += VoxelIncr * step;

			tile = getTile(CurrentV);

			// check if any of the values of T are nan
			if (isnan(MaxT.x) || isnan(MaxT.y) || isnan(MaxT.z) ||
				isinf(MaxT.x) || isinf(MaxT.y) || isinf(MaxT.z)) {
				return { vec3(0), blockType::OutOfBounds };
			}
		}

		//float min = MaxT.min_element();

		MaxT -= VoxelIncr * DeltaT;

		vec3 hit_position = SourcePos + (Dir * MaxT.min_element());

		return { hit_position, tile };

	}
};