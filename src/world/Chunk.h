#pragma once

#include "Block.h"
#include <string>

namespace electrodiux::voxel::world {

	constexpr int CHUNK_SIZE = 16;
	constexpr int BLOCKS_PER_CHUNK = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

	inline int calculateChunkIndex(int x, int y, int z) {
		return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
	}

	inline int calculateChunkX(int index) {
		return index % CHUNK_SIZE;
	}

	inline int calculateChunkY(int index) {
		return (index / CHUNK_SIZE) % CHUNK_SIZE;
	}

	inline int calculateChunkZ(int index) {
		return index / (CHUNK_SIZE * CHUNK_SIZE);
	}

	inline bool isOutOfBounds(int x, int y, int z) {
		return x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE;
	}

	struct ChunkLocation {
		int x, y, z;
		ChunkLocation(int x, int y, int z) : x(x), y(y), z(z) {}
	};

	class ChunkData {
		public:
		const ChunkLocation location;

		ChunkData(const ChunkLocation location) : location(location) {}

		virtual ~ChunkData() {}

		virtual void setBlock(int x, int y, int z, Block block) {}

		virtual Block getBlock(int x, int y, int z) const { return AIR; }
	};

	/*class PalleteChunkData : public ChunkData {
		private:

		typedef unsigned char PalleteIndex;

		struct PalleteBlock {
			Block block;
			PalleteIndex index;
			int count;
		};

		PalleteIndex blocks[BLOCKS_PER_CHUNK];
		std::vector<PalleteBlock> pallete;

		PalleteBlock getPalleteBlock(Block& block);

		public:
		~PalleteChunkData() override;

		void setBlock(int x, int y, int z, Block& block) override;

		Block& getBlock(int x, int y, int z) const override;

	};*/

	class ArrayChunkData : public ChunkData {
		private:
		unsigned char* blocks;

		public:
		ArrayChunkData(const ChunkLocation location) : ChunkData(location) {
			blocks = new unsigned char[BLOCKS_PER_CHUNK];
			std::memset(blocks, 0, BLOCKS_PER_CHUNK * sizeof(unsigned char));
		}

		~ArrayChunkData() override {
			delete[] blocks;
		}

		void setBlock(int x, int y, int z, Block block) override;

		Block getBlock(int x, int y, int z) const override;
	};

	class Chunk {
		private:
		ChunkData* data;

		public:
		Chunk(ChunkData* data) : data(data) {}
		~Chunk() { delete data; }

		void setBlock(int x, int y, int z, Block block);

		Block getBlock(int x, int y, int z) const;

		ChunkData* getChunkData() const;

		const ChunkLocation getChunkLocation() const;
	};

}