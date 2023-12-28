#pragma once

#include "BlockModel.h"
#include <string>

namespace electrodiux::voxel::block {

	typedef std::string BlockID;
	typedef unsigned char BlockRegistryID;

	struct BlockDefinition {

		BlockID id;
		BlockRegistryID registry_id;

		BlockModel* model;

		private:
		BlockDefinition(BlockID& id, BlockRegistryID registry_id, BlockModel* model) : id(id), registry_id(registry_id), model(model) { }

		friend class BlockDefinitionRegistryEntry;
	};

	class BlockDefinitionRegistryEntry {
		
		private:
		BlockID block_id;
		BlockRegistryID* registry_id_ptr; // If this is not null, the registry ID will be written to the memory location pointed to by this pointer

		BlockModel* block_model = nullptr;

		BlockDefinitionRegistryEntry(const BlockID& block_id, BlockRegistryID* registry_id_ptr = nullptr) : block_id(block_id), registry_id_ptr(registry_id_ptr) { }
		
		friend class BlockRegister;

		public:
		BlockDefinitionRegistryEntry* setModel(BlockModel* model) {
			this->block_model = model;
			return this;
		}

		BlockDefinition* create();
	};

	class CubeBlockModelRegistryEntry {

		private:
		BlockTexture textures[6];
		bool internal_faces = false;
		bool transparent = false;

		CubeBlockModelRegistryEntry(const BlockTexture& default_texture) {
			for (int i = 0; i < 6; i++) {
				textures[i] = default_texture;
			}
		}

		friend class BlockRegister;

		public:
		CubeBlockModelRegistryEntry* setTexture(int face, const BlockTexture& texture) {
			textures[face] = texture;
			return this;
		}

		CubeBlockModelRegistryEntry* setInternalFaces(bool internal_faces) {
			this->internal_faces = internal_faces;
			return this;
		}

		CubeBlockModelRegistryEntry* setTransparent(bool transparent) {
			this->transparent = transparent;
			return this;
		}
		
		BlockModel* create();

	};

	static class BlockRegister {

		public:
		static BlockDefinitionRegistryEntry registerBlock(const BlockID& block_id, BlockRegistryID* registry_id = nullptr);
		static unsigned int ammonutOfRegisteredBlocks();

		static CubeBlockModelRegistryEntry registerCubeBlockModel();
		static CubeBlockModelRegistryEntry registerCubeBlockModel(const BlockTexture& default_texture);

		static const BlockDefinition* getBlockDefinition(BlockRegistryID registry_id);

		static void destroy();

	};

}