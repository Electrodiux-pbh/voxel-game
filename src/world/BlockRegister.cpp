#include "BlockRegister.h"

#include <vector>

namespace electrodiux::voxel::block {

	static BlockRegistryID block_registry_id_counter = 1; // Don't use 0 as a registry ID because it is reserved for NO_BLOCK or AIR
	static BlockDefinition* block_definitions[256] = { nullptr };
	static std::vector<BlockModel*> block_models;

	BlockDefinitionRegistryEntry BlockRegister::registerBlock(const BlockID& block_id, BlockRegistryID* registry_id) {
		return BlockDefinitionRegistryEntry(block_id, registry_id);
	}

	CubeBlockModelRegistryEntry BlockRegister::registerCubeBlockModel() {
		return CubeBlockModelRegistryEntry(BlockTexture());
	}

	CubeBlockModelRegistryEntry BlockRegister::registerCubeBlockModel(const BlockTexture& default_texture) {
		return CubeBlockModelRegistryEntry(default_texture);
	}

	const BlockDefinition* BlockRegister::getBlockDefinition(BlockRegistryID registry_id) {
		return block_definitions[registry_id];
	}

	void BlockRegister::destroy() {
		for (BlockDefinition* definition : block_definitions) {
			delete definition;
		}

		for (BlockModel* model : block_models) {
			delete model;
		}

		block_models.clear();
	}

	BlockDefinition* BlockDefinitionRegistryEntry::create() {
		BlockDefinition* definition = new BlockDefinition(block_id, block_registry_id_counter, block_model);
		
		if (registry_id_ptr != nullptr) {
			*registry_id_ptr = block_registry_id_counter;
		}
		block_registry_id_counter++;

		block_definitions[definition->registry_id] = definition;

		return definition;
	}

	BlockModel* CubeBlockModelRegistryEntry::create() {
		BlockModel* block_model = new BlockModel(
		textures[FACE_TOP],
		textures[FACE_BOTTOM],
		textures[FACE_RIGHT],
		textures[FACE_LEFT],
		textures[FACE_FRONT],
		textures[FACE_BACK],
		internal_faces);
		
		block_models.push_back(block_model);
		
		return block_model;
	}

}