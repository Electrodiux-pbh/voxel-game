#pragma once

namespace electrodiux::math {

	struct vec3;

}

namespace electrodiux::voxel::world {

	typedef unsigned char Block;

}

namespace electrodiux::voxel::block {

	constexpr int FACE_TOP = 0;
	constexpr int FACE_BOTTOM = 1;
	constexpr int FACE_LEFT = 2;
	constexpr int FACE_RIGHT = 3;
	constexpr int FACE_FRONT = 4;
	constexpr int FACE_BACK = 5;

	#ifndef BLOCKS
	#define BLOCKS

	extern world::Block AIR;
	extern world::Block DIRT;
	extern world::Block GRASS;
	extern world::Block STONE;
	extern world::Block COBBLESTONE;
	extern world::Block OAK_PLANKS;
	extern world::Block STONE_BRICKS;
	extern world::Block OAK_LOG;
	extern world::Block GLASS;
	extern world::Block OAK_LEAVES;

	#endif

}