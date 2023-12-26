#pragma once

namespace electrodiux::voxel::world {

	class Chunk;

}

namespace electrodiux::voxel::gfx {

	class ChunkMesh {

	private:
	unsigned int vao_id;
	unsigned int vbo_id;
	unsigned int ebo_id;
	unsigned int indices_count;

	ChunkMesh(unsigned int vao_id, unsigned int vbo_id, unsigned int ebo_id, unsigned int indices_count);

	public:
	~ChunkMesh();

	void render();

	static ChunkMesh* createMesh(world::Chunk* chunk);

	};

}