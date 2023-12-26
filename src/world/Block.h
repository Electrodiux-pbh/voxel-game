#pragma once

namespace electrodiux::voxel::world {

	/*struct Block {
		int id;

		Block() : id(0) {}
		Block(int id) : id(id) {}

		bool operator== (const Block& other) {
			return id == other.id;
		}
	};*/
	typedef unsigned char Block;

	const Block AIR = Block(0);
	const Block DIRT = Block(1);

}