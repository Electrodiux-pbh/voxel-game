#include "Chunk.h"

namespace electrodiux::voxel::world {

    void Chunk::setBlock(int x, int y, int z, Block block) {
        this->data->setBlock(x, y, z, block);
    }

    Block Chunk::getBlock(int x, int y, int z) const {
        return this->data->getBlock(x, y, z);
    }

    ChunkData* Chunk::getChunkData() const {
        return this->data;
    }

    const ChunkLocation Chunk::getChunkLocation() const {
        return this->data->location;
    }
}
