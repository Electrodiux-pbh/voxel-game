#include "BlockModel.h"

namespace electrodiux::voxel::block {

	BlockTexture& atlasTexture(int atlas_face, int atlas_width, int atlas_height, BlockTexture& texture) {
		int x = atlas_face % atlas_width;
		int y = atlas_height - 1 - atlas_face / atlas_width;

		float texture_coords[8] = {
			x / (float)atlas_width, (y + 1) / (float)atlas_height,
			x / (float)atlas_width, y / (float)atlas_height,
			(x + 1) / (float)atlas_width, y / (float)atlas_height,
			(x + 1) / (float)atlas_width, (y + 1) / (float)atlas_height,
		};

		texture = BlockTexture(texture_coords);
		return texture;
	}

}