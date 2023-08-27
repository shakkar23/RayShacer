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
	World() :world(initWorld()) {
	}
	std::array<std::array<std::array<blockType, blockCount>, blockCount>, blockCount> initWorld();

	std::array<std::array<std::array<blockType, blockCount>, blockCount>, blockCount> world;

	struct blockInfo {
		vec3 where;
		blockType type;
	};

	blockInfo voxel_traversal_old(const vec3 ray_start, const vec3 ray_direction) const;




	blockInfo voxel_traversal_new(const vec3 SourcePos, const vec3 Dir) const;
};