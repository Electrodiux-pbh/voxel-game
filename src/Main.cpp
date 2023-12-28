#include "rendering/graphics_libs.h"
#include "rendering/render.h"
#include "world/BlockRegister.h"
#include "world/BlockModel.h"
#include "world/Block.h"
#include "input/input.h"

#include <iostream>

using namespace electrodiux;
using namespace electrodiux::voxel;

void loadBlocks() {
	block::BlockTexture dirt_texture;
	block::atlasTexture(1, 7, 2, dirt_texture);
	block::BlockRegister::registerBlock("dirt", (block::BlockRegistryID*)&block::DIRT).setModel(
		block::BlockRegister::registerCubeBlockModel(dirt_texture).create()
	)->create();

	block::BlockTexture stone_texture;
	block::atlasTexture(0, 7, 2, stone_texture);
	block::BlockRegister::registerBlock("stone", (block::BlockRegistryID*)&block::STONE).setModel(
		block::BlockRegister::registerCubeBlockModel(stone_texture).create()
	)->create();

	block::BlockTexture grass_top_texture;
	block::atlasTexture(3, 7, 2, grass_top_texture);
	block::BlockTexture grass_side_texture;
	block::atlasTexture(2, 7, 2, grass_side_texture);
	block::BlockRegister::registerBlock("grass", (block::BlockRegistryID*)&block::GRASS).setModel(
		block::BlockRegister::registerCubeBlockModel(grass_side_texture)
		.setTexture(block::FACE_TOP, grass_top_texture)
		->setTexture(block::FACE_BOTTOM, dirt_texture)
		->create()
	)->create();

	block::BlockTexture cobblestone_texture;
	block::atlasTexture(4, 7, 2, cobblestone_texture);
	block::BlockRegister::registerBlock("cobblestone", (block::BlockRegistryID*)&block::COBBLESTONE).setModel(
		block::BlockRegister::registerCubeBlockModel(cobblestone_texture).create()
	)->create();

	block::BlockTexture oak_planks_texture;
	block::atlasTexture(5, 7, 2, oak_planks_texture);
	block::BlockRegister::registerBlock("oak_planks", (block::BlockRegistryID*)&block::OAK_PLANKS).setModel(
		block::BlockRegister::registerCubeBlockModel(oak_planks_texture).create()
	)->create();

	block::BlockTexture stone_bricks_texture;
	block::atlasTexture(6, 7, 2, stone_bricks_texture);
	block::BlockRegister::registerBlock("stone_bricks", (block::BlockRegistryID*)&block::STONE_BRICKS).setModel(
		block::BlockRegister::registerCubeBlockModel(stone_bricks_texture).create()
	)->create();

	block::BlockTexture oak_log_top_texture;
	block::atlasTexture(7, 7, 2, oak_log_top_texture);
	block::BlockTexture oak_log_side_texture;
	block::atlasTexture(8, 7, 2, oak_log_side_texture);
	block::BlockRegister::registerBlock("oak_log", (block::BlockRegistryID*)&block::OAK_LOG).setModel(
		block::BlockRegister::registerCubeBlockModel(oak_log_side_texture)
		.setTexture(block::FACE_TOP, oak_log_top_texture)
		->setTexture(block::FACE_BOTTOM, oak_log_top_texture)
		->create()
	)->create();
}

int main() {
	int status = initialize();
	if (status != 0) {
		std::cout << "Failed to initialize" << std::endl;
		return status;
	}

	loadBlocks();

	loadResources();

	renderLoop();
	terminateRender();

	block::BlockRegister::destroy();

	delete input::mouse;
	delete input::keyboard;

	return 0;
}
