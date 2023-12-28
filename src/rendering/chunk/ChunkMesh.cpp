#include "../graphics_libs.h"

#include "ChunkMesh.h"
#include "../../world/Chunk.h"

#include "../../world/BlockRegister.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

constexpr int POSITION_POINTER = 0;
constexpr int POSITION_SIZE = 3;
constexpr int POSITION_OFFSET = 0;

constexpr int TEXTURE_COORD_POINTER = 1;
constexpr int TEXTURE_COORD_SIZE = 2;
constexpr int TEXTURE_COORD_OFFSET = POSITION_OFFSET + POSITION_SIZE * sizeof(GLfloat);

constexpr int VERTEX_SIZE = POSITION_SIZE + TEXTURE_COORD_SIZE;
constexpr int VERTEX_STRIDE = POSITION_SIZE * sizeof(GLfloat) + TEXTURE_COORD_SIZE * sizeof(GLfloat);

constexpr int VERTICES_PER_FACE = 4;
constexpr int VERTICES_PER_BLOCK = 6 * VERTICES_PER_FACE;

constexpr int FACE_FRONT = 0;
constexpr int FACE_BACK = 1;
constexpr int FACE_LEFT = 2;
constexpr int FACE_RIGHT = 3;
constexpr int FACE_TOP = 4;
constexpr int FACE_BOTTOM = 5;

constexpr char FACE_FRONT_MASK = 0b00000001;
constexpr char FACE_BACK_MASK =	0b00000010;
constexpr char FACE_LEFT_MASK =	0b00000100;
constexpr char FACE_RIGHT_MASK = 0b00001000;
constexpr char FACE_TOP_MASK = 0b00010000;
constexpr char FACE_BOTTOM_MASK = 0b00100000;

constexpr char NO_FACES_MASK = 0b00000000;
constexpr char ALL_FACES_MASK = 0b00111111;

constexpr GLfloat cube_vertices[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f
};

constexpr GLuint cube_indices[] = {
	// Front face
	0, 2, 1, 2, 0, 3,

	// Back face
	1, 2, 0, 3, 0, 2,

	// Left face
	0, 2, 1, 2, 0, 3,

	// Right face
	3, 0, 2, 1, 2, 0,

	// Top face
	0, 2, 1, 2, 0, 3,

	// Bottom face
	3, 0, 2, 1, 2, 0,
};

constexpr float cube_texture_coords[] = {
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
};

namespace electrodiux::voxel::gfx {

	ChunkMesh::ChunkMesh(unsigned int vao_id, unsigned int vbo_id, unsigned int ebo_id, unsigned int indices_count) : vao_id(vao_id), vbo_id(vbo_id), ebo_id(ebo_id), indices_count(indices_count) { }

	ChunkMesh::~ChunkMesh() {
		glDeleteBuffers(1, &vbo_id);
		glDeleteBuffers(1, &ebo_id);
		glDeleteVertexArrays(1, &vao_id);
	}

	void ChunkMesh::render() {
		glBindVertexArray(vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
	
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	bool isExposed(const block::BlockDefinition* other, const block::BlockDefinition* block) {
		if(other == nullptr) return true;
		if (other->model->isTransparent()) {
			if (other == block) {
				return block->model->hasInternalFaces();
			}
			return true;
		}
		return false;
	}

	char getExposedFaces(const world::ChunkData* data, int x, int y, int z, const block::BlockDefinition* block) {
		char exposed_faces = 0;

		if (isExposed(data->getBlockDefinition(x, y + 1, z), block))
			exposed_faces |= FACE_TOP_MASK;
		if (isExposed(data->getBlockDefinition(x, y - 1, z), block))
			exposed_faces |= FACE_BOTTOM_MASK;
		if (isExposed(data->getBlockDefinition(x, y, z - 1), block))
			exposed_faces |= FACE_FRONT_MASK;
		if (isExposed(data->getBlockDefinition(x, y, z + 1), block))
			exposed_faces |= FACE_BACK_MASK;
		if (isExposed(data->getBlockDefinition(x + 1, y, z), block))
			exposed_faces |= FACE_LEFT_MASK;
		if (isExposed(data->getBlockDefinition(x - 1, y, z), block))
			exposed_faces |= FACE_RIGHT_MASK;

		return exposed_faces;
	}

	void addFace(std::vector<GLfloat>& data_buffer, std::vector<GLuint>& indices_buffer, glm::mat4& transform, int face, int& faces_count, const block::BlockTexture& texture) {

		for (int i = 0; i < VERTICES_PER_FACE; i++) {
			int index = (face * VERTICES_PER_FACE + i) * POSITION_SIZE;
			glm::vec4 vertex = transform * glm::vec4(cube_vertices[index + 0], cube_vertices[index + 1], cube_vertices[index + 2], 1.0f);

			data_buffer.push_back(vertex.x);
			data_buffer.push_back(vertex.y);
			data_buffer.push_back(vertex.z);
			
			data_buffer.push_back(texture.texture_coords[i * TEXTURE_COORD_SIZE + 0]);
			data_buffer.push_back(texture.texture_coords[i * TEXTURE_COORD_SIZE + 1]);
		}

		int indices_offset = faces_count * VERTICES_PER_FACE;
		indices_buffer.push_back(indices_offset + cube_indices[face * 6 + 0]);
		indices_buffer.push_back(indices_offset + cube_indices[face * 6 + 1]);
		indices_buffer.push_back(indices_offset + cube_indices[face * 6 + 2]);

		indices_buffer.push_back(indices_offset + cube_indices[face * 6 + 3]);
		indices_buffer.push_back(indices_offset + cube_indices[face * 6 + 4]);
		indices_buffer.push_back(indices_offset + cube_indices[face * 6 + 5]);

		faces_count++;
	}

	ChunkMesh* ChunkMesh::createMesh(world::Chunk* chunk) {
		std::vector<GLfloat> data_buffer;
		std::vector<GLuint> indices_buffer;

		const world::ChunkData* data = chunk->getChunkData();

		int faces_count = 0;
		for (int x = 0; x < world::CHUNK_SIZE; x++) {
			for (int y = 0; y < world::CHUNK_SIZE; y++) {
				for (int z = 0; z < world::CHUNK_SIZE; z++) {
					world::Block block_id = data->getBlock(x, y, z);
					if (block_id != block::AIR) {
						const block::BlockDefinition* block_definition = block::BlockRegister::getBlockDefinition(block_id);
						if(block_definition == nullptr) continue;

						char exposed_faces = ALL_FACES_MASK;

						if (!block_definition->model->hasInternalFaces()) {
							exposed_faces = getExposedFaces(data, x, y, z, block_definition);
							if (exposed_faces == NO_FACES_MASK) {
								continue;
							}
						}

						glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x + data->location.x * world::CHUNK_SIZE, y + data->location.y * world::CHUNK_SIZE, z + data->location.z * world::CHUNK_SIZE));

						if (exposed_faces & FACE_FRONT_MASK) {
							addFace(data_buffer, indices_buffer, transform, FACE_FRONT, faces_count, block_definition->model->getFaceTexture(block::FACE_FRONT));
						}

						if (exposed_faces & FACE_BACK_MASK) {
							addFace(data_buffer, indices_buffer, transform, FACE_BACK, faces_count, block_definition->model->getFaceTexture(block::FACE_BACK));
						}

						if (exposed_faces & FACE_LEFT_MASK) {
							addFace(data_buffer, indices_buffer, transform, FACE_LEFT, faces_count, block_definition->model->getFaceTexture(block::FACE_LEFT));
						}

						if (exposed_faces & FACE_RIGHT_MASK) {
							addFace(data_buffer, indices_buffer, transform, FACE_RIGHT, faces_count, block_definition->model->getFaceTexture(block::FACE_RIGHT));
						}

						if (exposed_faces & FACE_TOP_MASK) {
							addFace(data_buffer, indices_buffer, transform, FACE_TOP, faces_count, block_definition->model->getFaceTexture(block::FACE_TOP));
						}

						if (exposed_faces & FACE_BOTTOM_MASK) {
							addFace(data_buffer, indices_buffer, transform, FACE_BOTTOM, faces_count, block_definition->model->getFaceTexture(block::FACE_BOTTOM));
						}
					}
				}
			}
		}

		unsigned int vao_id, vbo_id, ebo_id;

		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, data_buffer.size() * sizeof(GLfloat), data_buffer.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
		int indices_count = (int) indices_buffer.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(GLuint), indices_buffer.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(POSITION_POINTER, POSITION_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (GLvoid*) POSITION_OFFSET);
		glVertexAttribPointer(TEXTURE_COORD_POINTER, TEXTURE_COORD_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (GLvoid*) TEXTURE_COORD_OFFSET);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

		return new ChunkMesh(vao_id, vbo_id, ebo_id, indices_count);
	}

}