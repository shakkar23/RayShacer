#include "GameManager.hpp"
#include "../Rendering/Rendering.hpp"


#include <algorithm>
#include <cmath>
#include <iostream>
#include <array>
#include <execution>
#define _USE_MATH_DEFINES
#include <math.h>
void gameManager::Init(RenderWindow& window) {
	grass.load(window, "assets/grass.png");
	// init 480 by 720 windowBuffer

}

void gameManager::gameLogic(int x, int y, moveDirection prev, moveDirection curr) {
	playerYaw += x * 0.4f;
	playerPitch += y * 0.4f;

	playerYaw = fmod(playerYaw, 360.0f);
	playerPitch = fmod(playerPitch, 360.0f);

	if (curr.Left) {
		playerX -= 0.01f * cos(playerYaw * M_PI / 180.0f);
		playerZ += 0.01f * sin(playerYaw * M_PI / 180.0f);
	}
	if (curr.Right)
	{
		playerX += 0.01f * cos(playerYaw * M_PI / 180.0f);
		playerZ -= 0.01f * sin(playerYaw * M_PI / 180.0f);
	}


	if (curr.Jump)
		playerY += 0.01f;
	if (curr.Run)
		playerY -= 0.01f;
	// clamp player to blockCount
	playerX = std::clamp(playerX, 0.0f, (float)blockCount);
	playerZ = std::clamp(playerZ, 0.0f, (float)blockCount);
	playerY = std::clamp(playerY, 0.0f, (float)blockCount);


	if (curr.Down)
	{
		playerX -= 0.01f * sin(playerYaw * M_PI / 180.0f);
		playerZ -= 0.01f * cos(playerYaw * M_PI / 180.0f);
	}
	if (curr.Up) {
		playerX += 0.01f * sin(playerYaw * M_PI / 180.0f);
		playerZ += 0.01f * cos(playerYaw * M_PI / 180.0f);
	}

	//int shortestSide = std::min(winSizeX, winSizeY);

}
const uint32_t GetRand(uint32_t upperBound) {
	static unsigned long long PPTRNG = 0;

	PPTRNG = PPTRNG * 0x5d588b65 + 0x269ec3;
	uint32_t uVar1 = PPTRNG >> 0x10;
	if (upperBound != 0) {
		uVar1 = uVar1 * upperBound >> 0x10;
	}
	return uVar1;
}
/*
let d = Vec3::from(
	looking
		* Vec3A::new(
			((float)x - halfwidth) / halfheight,
			(halfheight - (float)y) / halfheight,
			1.0,
		)
		.normalize(),
);
*/

// raytrace
ShakColor gameManager::RenderPixel(const vec2 coord) const {

	constexpr auto FOV = 90.0f;

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
	do{
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
	}while (i < MAX_BOUNCES && block_info.type == Mirror);

	return pixel;
}
/*
ShakColor gameManager::RenderPixel(vec2 coord) {
	SDL_HINT_RENDER_OPENGL_SHADERS;
	// window stretching
	float aspectRatio = (float)winSizeX / (float)winSizeY;
	coord.x *= aspectRatio;

	ShakColor color;// = ShakColor((Uint8((float)x / winSizeX) * 255), Uint8(((float)y / winSizeY) * 255), std::max(color.r, color.g), (Uint8)255);
	color.r = 30;
	color.g = 30;
	color.b = 15;
	color.a = 255;


	// (bx^2 + by^2)t^2 + (2(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0

	// a = ray origin
	// b = ray direction
	// t = hit distance
	// r = radius of sphere

	// convert player angles to a looking vector
	constexpr auto FOV = 90;
	vec3 eyePos = vec3(0, 0, (float)-1/tan(toRadians(FOV/2)));

	vec3 rayDirection = (vec3(coord.x, coord.y, 0) - eyePos).normalize().rotateYP(playerYaw, playerPitch);

	vec3 rayOrigin(playerX, playerY, playerZ);
	vec3 lightDir = vec3(0, -1, 0);
	vec3 sphereOrigin = vec3(0, 0, 0.0f);

	lightDir.normalizeThis();

	float radius = 0.5f;

	float a = rayDirection.dotThis(rayDirection);
	float b = 2.0f * rayOrigin.dotThis(rayDirection);
	float c = rayOrigin.dotThis(rayOrigin) - radius * radius ;

	// quadratic formula discriminant
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0.0f) {

		// ray intersects sphere
		auto sqrtdesc = sqrt(discriminant);
		float t0 = (-b - sqrtdesc) / (2.0f * a);
		float t1 = (-b + sqrtdesc) / (2.0f * a);

		vec3 hitPoint1 = rayOrigin + rayDirection * t0;
		vec3 hitPoint2 = rayOrigin + rayDirection * t1;

		vec3 hitPoint;
		// if hitPoint is behind the screen then we don't want to draw it
		bool t0renderable = t0 > 0.0f;
		bool t1renderable = t1 > 0.0f;
		if (!t0renderable && !t1renderable)
			return color;
		else if (t0renderable && !t1renderable)
			hitPoint = hitPoint1;
		else if (!t0renderable && t1renderable)
			hitPoint = hitPoint2;
		else if (t0renderable && t1renderable)
		{
			hitPoint = hitPoint1;
			if (hitPoint1 - rayOrigin > hitPoint2 - rayOrigin) {
				hitPoint = hitPoint2;
			}
		}

		vec3 normal = hitPoint - sphereOrigin;// -vec3(0.0f, 0.0f, 0.0f);
		float light = std::max(normal.dotThis(-lightDir), 0.0f);
		normal.normalizeThis();

		normal = normal * 0.5f + 0.5f;
		color.r = normal.x * light * 255;
		color.g = normal.y * light * 255;
		color.b = normal.z * light * 255;
		color.a = 255;
	}

	return color;
}*/



void gameManager::render(RenderWindow& window) {
	window.clear();

	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	auto SWindow = SDL_GetWindowSurface(window.getWindow());
	//window.clear();

	auto prevSizeX = winSizeX;
	auto prevSizeY = winSizeY;

	window.getWindowSize(winSizeX, winSizeY);

	// get the shortest side of the window
	//int shortestSide = std::min(winSizeX, winSizeY);
	//std::vector<SDL_Point> renderBuffer;
	//constexpr int i = 720 * 480;
	//renderBuffer.reserve(winSizeX * winSizeY);




	//Uint8 r{1}, g{1}, b{1}, a = 255;
	/*
	auto work = [](const gameManager* that, SDL_Surface* SWindow, int windowDivided, int whichPart) {

		for (int j = 0, vj = SWindow->h - 1; j < SWindow->h; j++, --vj) {
			for (int i = (SWindow->w / windowDivided) * whichPart; i < (SWindow->w / windowDivided) * (whichPart + 1); i++) {

				vec2 coord = vec2((float)i / that->winSizeX, (float)j / that->winSizeY);
				coord = coord * 2.0f - 1.0f; // convert to range [-1, 1]

				// window stretching
				float aspectRatio = (float)that->winSizeX / (float)that->winSizeY;
				coord.x *= aspectRatio;

				ShakColor* target_pixel = (ShakColor*)((Uint8*)SWindow->pixels
					+ vj * SWindow->pitch
					+ i * SWindow->format->BytesPerPixel);

				(*target_pixel) = that->RenderPixel(coord);
			}
		}
	};
	*/


	// the window is divided up into 4 parts
	// each part is rendered in a separate thread
	/*
	const int windowDivided = window.numCoreCount / 4;
	{
		std::vector<std::jthread*> threads;
		threads.resize(windowDivided);

		for (int i = 0; i < windowDivided; i++) {
			threads[i] = new std::jthread(work, this, SWindow, windowDivided, i);
		}

		for (int i = 0; i < windowDivided; i++) {
			threads[i]->join();
			delete threads[i];
		}
	}*/

	if (prevSizeX != winSizeX) {
		horizontal_iterator.resize(winSizeX);

		for (uint32_t i = 0; i < winSizeX; i++)
			horizontal_iterator[i] = i;
	}
	if (prevSizeY != winSizeY)
	{
		vertical_iter.resize(winSizeY);

		for (uint32_t i = 0; i < winSizeY; i++)
			vertical_iter[i] = i;
	}

	std::for_each(std::execution::par, vertical_iter.begin(), vertical_iter.end(),
		[this, SWindow](int y) {
			std::for_each(std::execution::par, horizontal_iterator.begin(), horizontal_iterator.end(),
			[this, SWindow, y](int x) {
					vec2 coord = vec2((float)x / this->winSizeX, (float)y / this->winSizeY);
	coord = coord * 2.0f - 1.0f; // convert to range [-1, 1]

	// window stretching
	float aspectRatio = (float)this->winSizeX / (float)this->winSizeY;
	coord.x *= aspectRatio;
	auto ry = (winSizeY - y) - 1;
	ShakColor* target_pixel = (ShakColor*)((Uint8*)SWindow->pixels
		+ ry * SWindow->pitch
		+ x * SWindow->format->BytesPerPixel);


	(*target_pixel) = this->RenderPixel(coord);
				});
		});

	window.renderSurface(SWindow, NULL, NULL);
	//window.SetRenderDrawColor(rand() % 255, rand() % 255, rand() % 255, 255);

	window.display();

	//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	//std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	//std::cout << "It took me " << time_span.count() << " seconds.";
	//std::cout << std::endl;


}
gameManager GameManager;