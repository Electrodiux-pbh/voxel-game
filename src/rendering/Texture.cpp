#include "Texture.h"

#include <iostream>
#include <stb/stb_image.h>
#include <cmath>

namespace electrodiux::voxel::gfx {

	Texture* loadTexture(void* image_pixels, int width, int height, GLint image_format, GLint filter, bool mipmaps, float antisotropic_ext) {
		GLuint texture_id;
		glGenTextures(1, &texture_id);

		glBindTexture(GL_TEXTURE_2D, texture_id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glTexImage2D(GL_TEXTURE_2D, 0, image_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, image_pixels);

		if (mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);

			int mipmap_filter = filter == GL_NEAREST ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_filter);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

			if (GL_EXT_texture_filter_anisotropic) {
				GLfloat max_anisotropic;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropic);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(antisotropic_ext, max_anisotropic));
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return new Texture(texture_id);
	}

	Texture* loadTextureFromFile(const char* path, GLint filter, bool mipmaps, float antisotropic_ext) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);

		unsigned char* image_pixels = stbi_load(path, &width, &height, &channels, 0);

		if (image_pixels == nullptr) {
			throw std::runtime_error("Failed to load texture from file " + std::string(path));
		}

		GLint image_format = channels == 3 ? GL_RGB : GL_RGBA;
		Texture* texture = loadTexture(image_pixels, width, height, image_format, filter, mipmaps, antisotropic_ext);

		stbi_image_free(image_pixels);

		return texture;
	}

	Texture::Texture(GLuint texture_id) : texture_id(texture_id) { }

	Texture::~Texture() {
		glDeleteTextures(1, &this->texture_id);
	}

	void Texture::bind() const {
		glBindTexture(GL_TEXTURE_2D, this->texture_id);
	}

	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint Texture::getTextureId() const {
		return this->texture_id;
	}

}

