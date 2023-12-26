#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>

namespace electrodiux::voxel::gfx {

	GLint compileShader(const char* shader_src, GLenum shader_type) {
		GLint shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &shader_src, nullptr);
		glCompileShader(shader_id);

		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if(!success) {
			GLchar info_log[512];
			glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
			std::cerr << "Error: Shader compilation failed\n" << info_log << std::endl;
			glDeleteShader(shader_id);
			return 0;
		}

		return shader_id;
	}

	GLint compileShaderProgram(const GLint* shaders, int shader_count) {
		GLint shader_program_id = glCreateProgram();

		for(int i = 0; i < shader_count; i++) {
			glAttachShader(shader_program_id, shaders[i]);
		}

		glLinkProgram(shader_program_id);

		GLint success;
		glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

		if(!success) {
			GLchar info_log[512];
			glGetProgramInfoLog(shader_program_id, 512, nullptr, info_log);
			std::cerr << "Error: Shader program linking failed\n" << info_log << std::endl;
			glDeleteProgram(shader_program_id);
			return 0;
		}

		for(int i = 0; i < shader_count; i++) {
			glDeleteShader(shaders[i]);
		}

		return shader_program_id;
	}

	ShaderProgram* createShaderProgramFromRaw(const char* vertex_src, const char* fragment_src) {
		GLint vertex_shader_id = compileShader(vertex_src, GL_VERTEX_SHADER);
		GLint fragment_shader_id = compileShader(fragment_src, GL_FRAGMENT_SHADER);

		if (vertex_shader_id == 0 || fragment_shader_id == 0) {
			return nullptr;
		}

		GLint shaders[] = { vertex_shader_id, fragment_shader_id };
		GLint shader_program_id = compileShaderProgram(shaders, 2);

		if(shader_program_id == 0) {
			return nullptr;
		}

		return new ShaderProgram(shader_program_id);
	}

	std::string readFile(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			throw std::runtime_error("Error: Could not open file: " + filename);
		}

		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		return content;
	}

	ShaderProgram* createShaderProgramFromFile(const char* vertex_path, const char* fragment_path)
	{
		try {
			std::string vertex_src = readFile(vertex_path);
			std::string fragment_src = readFile(fragment_path);

			return createShaderProgramFromRaw(vertex_src.c_str(), fragment_src.c_str());
		} catch (std::runtime_error& err) {
			std::cerr << err.what() << std::endl;
			return nullptr;
		}
	}

	ShaderProgram::~ShaderProgram() {
		this->detach();
		glDeleteProgram(this->shader_program);
	}

	GLint current_program = 0;

	void ShaderProgram::use() const {
		if (current_program != this->shader_program) {
			glUseProgram(this->shader_program);
			current_program = this->shader_program;
		}
	}

	void ShaderProgram::detach() const {
		current_program = 0;
		glUseProgram(0);
	}

	GLint ShaderProgram::getUniformLocation(const char* name) const {
		return glGetUniformLocation(this->shader_program, name);
	}

	GLint ShaderProgram::getShaderProgram() const {
		return this->shader_program;
	}

	void ShaderProgram::setMat4(const char* name, glm::mat4 value) const {
		use();
		glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderProgram::setVec3(const char* name, glm::vec3 value) const {
		use();
		glUniform3fv(this->getUniformLocation(name), 1, glm::value_ptr(value));
	}

	void ShaderProgram::setFloat(const char* name, float value) const
	{
		use();
		glUniform1f(this->getUniformLocation(name), value);
	}

	void ShaderProgram::setInt(const char* name, int value) const
	{
		use();
		glUniform1i(this->getUniformLocation(name), value);
	}

	void ShaderProgram::setBool(const char* name, bool value) const
	{
		use();
		glUniform1i(this->getUniformLocation(name), (int) value);
	}

}
