#pragma once

#include "graphics_libs.h"
#include "glm/glm.hpp"

namespace electrodiux::voxel::gfx {

	class ShaderProgram {
		
		private:
		GLint shader_program;
		
		public:
		ShaderProgram(GLint shader_program) : shader_program(shader_program) {}
		~ShaderProgram();

		void use() const;
		void detach() const;

		GLint getUniformLocation(const char* name) const;
		GLint getShaderProgram() const;

		void setMat4(const char* name, glm::mat4 value) const;
		void setVec3(const char* name, glm::vec3 value) const;

		void setFloat(const char* name, float value) const;
		void setInt(const char* name, int value) const;
		void setBool(const char* name, bool value) const;

	};

	ShaderProgram* createShaderProgramFromRaw(char* vertex_src, char* fragment_src);
	ShaderProgram* createShaderProgramFromFile(const char* vertex_path, const char* fragment_path);

}