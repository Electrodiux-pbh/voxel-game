#include "Chunk.h"

namespace electrodiux::voxel::world {

    void ArrayChunkData::setBlock(int x, int y, int z, Block block) {
        if(isOutOfBounds(x, y, z)) return;
        this->blocks[calculateChunkIndex(x, y, z)] = block;
    }

    Block ArrayChunkData::getBlock(int x, int y, int z) const {
        if(isOutOfBounds(x, y, z)) return block::AIR;
        Block block = Block(this->blocks[calculateChunkIndex(x, y, z)]);
        return block;
    }

    const block::BlockDefinition* ArrayChunkData::getBlockDefinition(int x, int y, int z) const {
        if (isOutOfBounds(x, y, z)) return nullptr;
        return block::BlockRegister::getBlockDefinition(this->blocks[calculateChunkIndex(x, y, z)]);
    }

}