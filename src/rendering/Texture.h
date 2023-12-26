#pragma once

#include "graphics_libs.h"

namespace electrodiux::voxel::gfx {

	constexpr float DEFAULT_ANISOTROPIC_EXT = 4.0f;

	class Texture {
		private:
		GLuint texture_id;

		public:
		Texture(GLuint texture_id);
		~Texture();

		void bind() const;
		void unbind() const;

		GLuint getTextureId() const;
	};

	Texture* loadTexture(void* image_pixels, int width, int height, GLint image_format, GLint filter, bool mipmaps = true, float antisotropic_ext = DEFAULT_ANISOTROPIC_EXT);
	Texture* loadTextureFromFile(const char* path, GLint filter, bool mipmaps = true, float antisotropic_ext = DEFAULT_ANISOTROPIC_EXT);

}