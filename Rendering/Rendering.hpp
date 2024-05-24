#pragma once
#include <vector>
#include <span>

#include "../Utility/vec2.hpp"
#include "../Utility/Color.hpp"
#include "../World/World.hpp"

struct player_info {

	float playerX;
	float playerY;
	float playerZ;

	float playerYaw;
	float playerPitch;
};

void wrapper_thing(const World& world, const std::span<vec2> coords, std::span<ShakColor> colors, player_info& info);
