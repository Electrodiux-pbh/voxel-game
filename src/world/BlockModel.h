#pragma once

#include "Block.h"

namespace electrodiux::voxel::block {

	struct BlockTexture {
		float texture_coords[8];

		BlockTexture() {
			for (int i = 0; i < 8; i++) {
				texture_coords[i] = 0.0f;
			}
		}

		BlockTexture(float texture_coords[8]) {
			for (int i = 0; i < 8; i++) {
				this->texture_coords[i] = texture_coords[i];
			}
		}
	};

	BlockTexture& atlasTexture(int atlas_face, int atlas_width, int atlas_height, BlockTexture& texture);

	class BlockModel {

		private:
		BlockTexture* textures;

		bool internal_faces;
		bool transparent;

		public:
		BlockModel(const BlockTexture& top, const BlockTexture& bottom, const BlockTexture& right, const BlockTexture& left, const BlockTexture& front, const BlockTexture& back, bool internal_faces, bool transparent) {
			BlockTexture* textures = new BlockTexture[6];

			textures[FACE_TOP] = top;
			textures[FACE_BOTTOM] = bottom;
			textures[FACE_RIGHT] = right;
			textures[FACE_LEFT] = left;
			textures[FACE_FRONT] = front;
			textures[FACE_BACK] = back;

			this->textures = textures;
			this->internal_faces = internal_faces;
			this->transparent = transparent;
		}

		~BlockModel() {
			delete[] textures;
		}

		BlockTexture getFaceTexture(int face) const {
			return textures[face];
		}

		bool hasInternalFaces() const {
			return internal_faces;
		}

		bool isTransparent() const {
			return transparent;
		}

	};

}

