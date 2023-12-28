#include "graphics_libs.h"
#include "render.h"

#include "Window.h"
#include "Shader.h"
#include "Color.h"
#include "Camera.h"
#include "Texture.h"

#include "chunk/ChunkMesh.h"
#include "../world/Chunk.h"

#include "../input/input.h"

#include <iostream>
#include <cmath>
#include <limits>

#include "../physics/physics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../world/BlockRegister.h"

using namespace electrodiux::voxel;

namespace electrodiux {

	constexpr float border_vertices[] = {
		-0.0001f, -0.0001f, -0.0001f,  // Vertex 0
		1.0001f, -0.0001f, -0.0001f,   // Vertex 1
		1.0001f, 1.0001f, -0.0001f,    // Vertex 2
		-0.0001f, 1.0001f, -0.0001f,   // Vertex 3
		-0.0001f, -0.0001f, 1.0001f,   // Vertex 4
		1.0001f, -0.0001f, 1.0001f,    // Vertex 5
		1.0001f, 1.0001f, 1.0001f,     // Vertex 6
		-0.0001f, 1.0001f, 1.0001f     // Vertex 7
	};

	constexpr unsigned int border_indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,  // Bottom face
		4, 5, 5, 6, 6, 7, 7, 4,  // Top face
		0, 4, 1, 5, 2, 6, 3, 7   // Connecting lines
	};

	class BlockBorder {

		static constexpr int indices_count = 24;

		private:
		unsigned int vao_id;
		unsigned int vbo_id;
		unsigned int ebo_id;

		public:
		BlockBorder() {
			glGenVertexArrays(1, &vao_id);
			glBindVertexArray(vao_id);

			glGenBuffers(1, &vbo_id);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 8, border_vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &ebo_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_count, border_indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		}

		~BlockBorder() {
			glDeleteBuffers(1, &vbo_id);
			glDeleteBuffers(1, &ebo_id);
			glDeleteVertexArrays(1, &vao_id);
		}

		void render() {
			glBindVertexArray(vao_id);
			glEnableVertexAttribArray(0);
			glLineWidth(2.0f);
			glDrawElements(GL_LINES, indices_count, GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}

	};

	gfx::ShaderProgram* border_shader;
	BlockBorder* border;

	gfx::Window* window;
	gfx::Camera* camera;
	gfx::ShaderProgram* shader;
	gfx::Texture* texture;

	math::vec3 getRaycastedBlock(const math::vec3& origin, const math::vec3& direction, world::Chunk* chunk) {
		math::vec3 closest_block = math::vec3(std::numeric_limits<float>::quiet_NaN());
		float closest_distance = std::numeric_limits<float>::max();

		for (int x = 0; x < world::CHUNK_SIZE; x++) {
			for (int y = 0; y < world::CHUNK_SIZE; y++) {
				for (int z = 0; z < world::CHUNK_SIZE; z++) {
					if (chunk->getBlock(x, y, z) != block::AIR) {
						world::ChunkLocation chunk_loc = chunk->getChunkLocation();
						math::vec3 block_pos = math::vec3(x + chunk_loc.x * world::CHUNK_SIZE, y + chunk_loc.y * world::CHUNK_SIZE, z + chunk_loc.z * world::CHUNK_SIZE);
						
						if (!phy::intersects(phy::raycastFromAnglesDeg(origin, direction.x, direction.y), phy::AABB(block_pos, block_pos + math::vec3(1.0f)))) {
							continue;
						}

						float distance = math::distance(origin, block_pos);
						if (distance < closest_distance) {
							closest_block = block_pos;
							closest_distance = distance;
						}
					}
				}
			}
		}

		return closest_block;
	}

	math::vec3 getAdjacentRaycastedBlock(const math::vec3& origin, const math::vec3& direction, world::Chunk* chunk) {
		math::vec3 closest_block = math::vec3(std::numeric_limits<float>::quiet_NaN());
		float closest_distance = std::numeric_limits<float>::max();

		for (int x = 0; x < world::CHUNK_SIZE; x++) {
			for (int y = 0; y < world::CHUNK_SIZE; y++) {
				for (int z = 0; z < world::CHUNK_SIZE; z++) {
					if (chunk->getBlock(x, y, z) != block::AIR) {

						world::ChunkLocation chunk_loc = chunk->getChunkLocation();
						math::vec3 block_pos = math::vec3(x + chunk_loc.x * world::CHUNK_SIZE, y + chunk_loc.y * world::CHUNK_SIZE, z + chunk_loc.z * world::CHUNK_SIZE);

						phy::AABBface face = phy::intersectsFace(phy::raycastFromAnglesDeg(origin, direction.x, direction.y), phy::AABB(block_pos, block_pos + math::vec3(1.0f)));

						if (face == 0) {
							continue;
						}

						float distance = math::distance(origin, block_pos);
						if (distance < closest_distance) {
							math::vec3 hit_normal = math::vec3(0.0f);

							if (face == phy::FACE_BOTTOM) {
								hit_normal = math::vec3(0.0f, -1.0f, 0.0f);
							}
							else if (face == phy::FACE_TOP) {
								hit_normal = math::vec3(0.0f, 1.0f, 0.0f);
							}
							else if (face == phy::FACE_LEFT) {
								hit_normal = math::vec3(-1.0f, 0.0f, 0.0f);
							}
							else if (face == phy::FACE_RIGHT) {
								hit_normal = math::vec3(1.0f, 0.0f, 0.0f);
							}
							else if (face == phy::FACE_FRONT) {
								hit_normal = math::vec3(0.0f, 0.0f, -1.0f);
							}
							else if (face == phy::FACE_BACK) {
								hit_normal = math::vec3(0.0f, 0.0f, 1.0f);
							}

							closest_block = block_pos + hit_normal;
							closest_distance = distance;
						}
					}
				}
			}
		}

		return closest_block;
	}

	void cameraMove(gfx::Camera* camera, float deltaTime) {
		float x = 0, y = 0, z = 0;

		const float cos = std::cos(glm::radians(camera->rotation.y));
		const float sin = std::sin(glm::radians(camera->rotation.y));

		if (input::keyboard->isKeyPressed(GLFW_KEY_W)) {
			x += sin;
			z -= cos;
		}

		if (input::keyboard->isKeyPressed(GLFW_KEY_S)) {
			x -= sin;
			z += cos;
		}

		if (input::keyboard->isKeyPressed(GLFW_KEY_A)) {
			x -= cos;
			z -= sin;
		}

		if (input::keyboard->isKeyPressed(GLFW_KEY_D)) {
			x += cos;
			z += sin;
		}

		if (input::keyboard->isKeyPressed(GLFW_KEY_SPACE)) {
			y++;
		}

		if (input::keyboard->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			y--;
		}

		float speed = 4.317f;
		if (input::keyboard->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
			speed = 7.0f;
			camera->setFov(130.0f);
		} else {
			camera->setFov(120.0f);
		}

		camera->position += (glm::vec3(x, y, z) * deltaTime * speed);
	}

	float mouseSensitivity = 4.5f;

	void cameraRotate(gfx::Camera* camera, float deltaTime) {
		camera->rotation.x += (float)(input::mouse->getDY() * deltaTime * mouseSensitivity);
		camera->rotation.y += (float)(input::mouse->getDX() * deltaTime * mouseSensitivity);
	}
	
	float aspect_ratio = 800.0f / 600.0f;

	void sizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		aspect_ratio = width / (float)height;
		camera->setAspect(aspect_ratio);
	}

	void cursorPositionCallback(GLFWwindow* window, double x, double y) {
		input::mouse->positionCallback(x, y);
	}

	void cursorScrollCallback(GLFWwindow* window, double x, double y) {
		input::mouse->scrollCallback(x, y);
	}

	void cursorButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		input::mouse->buttonCallback(button, action, mods);
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		input::keyboard->keyCallback(key, scancode, action, mods);
	}

	int initialize() {
		if (!glfwInit()) {
			std::cout << "Failed to initialize GLFW" << std::endl;
			return -1;
		}

		window = new gfx::Window(800, 600, "Electrodiux Voxel");

		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			return -1;
		}

		input::mouse = new input::Mouse();
		glfwSetCursorPosCallback(window->getGLFWWindow(), cursorPositionCallback);
		glfwSetScrollCallback(window->getGLFWWindow(), cursorScrollCallback);
		glfwSetMouseButtonCallback(window->getGLFWWindow(), cursorButtonCallback);

		window->setInputMode(gfx::CURSOR, gfx::CURSOR_DISABLED);

		input::keyboard = new input::Keyboard();
		glfwSetKeyCallback(window->getGLFWWindow(), keyCallback);

		return 0;
	}

	world::Chunk* chunk;
	gfx::ChunkMesh* mesh;

	void updateChunkMesh() {
		delete mesh;
		mesh = gfx::ChunkMesh::createMesh(chunk);
	}

	void renderCrosshair() {
		constexpr float crosshairSize = 0.025f;

		glBegin(GL_LINES);
		glVertex2f(-crosshairSize / aspect_ratio, 0.0f);
		glVertex2f(crosshairSize / aspect_ratio, 0.0f);
		glVertex2f(0.0f, -crosshairSize);
		glVertex2f(0.0f, crosshairSize);
		glEnd();
	}

	void renderBlockBorder() {
		border_shader->use();
		glLineWidth(2.0f);

		math::vec3 cam_pos = math::vec3(camera->position.x, camera->position.y, camera->position.z);
		math::vec3 cam_dir = math::vec3(camera->rotation.x, camera->rotation.y, camera->rotation.z);
		math::vec3 pos = getRaycastedBlock(cam_pos, cam_dir, chunk);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));

		border_shader->setMat4("projection", camera->getProjectionMatrix());
		border_shader->setMat4("view", camera->calculateViewMatrix());
		border_shader->setMat4("transform", transform);

		border->render();

		border_shader->detach();
	}

	void render() {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		texture->bind();

		shader->use();
		mesh->render();
		shader->detach();

		texture->unbind();

		renderBlockBorder();

		glDisable(GL_DEPTH_TEST);
		renderCrosshair();
	}

	world::Block selected_block;

	void renderLoop() {
		selected_block = block::DIRT;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		double nextTime = glfwGetTime() + 1;

		double lastTime = glfwGetTime();
		while(!window->shouldClose()) {

			double currentTime = glfwGetTime();
			float deltaTime = (float)(currentTime - lastTime);
			lastTime = currentTime;

			glfwPollEvents();

			if (input::keyboard->isKeyPressed(GLFW_KEY_ESCAPE)) {
				window->close();
			}

			if (!input::keyboard->isKeyPressed(GLFW_KEY_F1)) {
				if (input::mouse->isButtonClicked(input::MOUSE_BUTTON_LEFT)) {
					world::ChunkLocation chunk_loc = chunk->getChunkLocation();
					math::vec3 cam_pos = math::vec3(camera->position.x, camera->position.y, camera->position.z);
					math::vec3 cam_dir = math::vec3(camera->rotation.x, camera->rotation.y, camera->rotation.z);
					math::vec3 pos = getRaycastedBlock(cam_pos, cam_dir, chunk);
					chunk->setBlock((int)pos.x - (chunk_loc.x * world::CHUNK_SIZE), (int)pos.y - (chunk_loc.y * world::CHUNK_SIZE), (int)pos.z - (chunk_loc.z * world::CHUNK_SIZE), block::AIR);
					updateChunkMesh();
				}

				if (input::mouse->isButtonClicked(input::MOUSE_BUTTON_RIGHT)) {
					world::ChunkLocation chunk_loc = chunk->getChunkLocation();
					math::vec3 cam_pos = math::vec3(camera->position.x, camera->position.y, camera->position.z);
					math::vec3 cam_dir = math::vec3(camera->rotation.x, camera->rotation.y, camera->rotation.z);
					math::vec3 pos = getAdjacentRaycastedBlock(cam_pos, cam_dir, chunk);
					chunk->setBlock((int)pos.x - (chunk_loc.x * world::CHUNK_SIZE), (int)pos.y - (chunk_loc.y * world::CHUNK_SIZE), (int)pos.z - (chunk_loc.z * world::CHUNK_SIZE), selected_block);
					updateChunkMesh();
				}

				if (input::keyboard->isKeyTyped(GLFW_KEY_E)) {
					selected_block++;
					if (selected_block > 7) {
						selected_block = block::DIRT;
					}
				}

				shader->setMat4("projection", camera->getProjectionMatrix());
				shader->setMat4("view", camera->calculateViewMatrix());

				glClearColor(103.0f / 255.0f, 224.0f / 255.0f, 235.0f / 255.0f, 1.0f);

				render();

				cameraRotate(camera, deltaTime);
				cameraMove(camera, deltaTime);
			}

			input::mouse->update();
			input::keyboard->update();

			window->swapBuffers();

			if (glfwGetTime() >= nextTime) {
				std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
				nextTime += 1.0f;
			}
		}
	}
	
	void loadResources() {
		window->setKeyCallback(keyCallback);
		window->setSizeCallback(sizeCallback);

		camera = new gfx::Camera(70, aspect_ratio, 0.1f, 100.0f);
		camera->position = glm::vec3(-1.0f, 0.0f, -1.0f);
		camera->rotation = glm::vec3(0.0f, 90.0f, 0.0f);

		shader = gfx::createShaderProgramFromFile("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
		texture = gfx::loadTextureFromFile("resources/textures/atlas.png", GL_NEAREST, false);

		border_shader = gfx::createShaderProgramFromFile("resources/shaders/border_vertex.glsl", "resources/shaders/border_fragment.glsl");
		border_shader->use();
		border_shader->setVec3("color", glm::vec3(0.0f, 0.0f, 0.0f));
		border_shader->detach();
		border = new BlockBorder();

		world::ChunkData* data = new world::ArrayChunkData(world::ChunkLocation(0, 0, 0));
		chunk = new world::Chunk(data);

		for (int x = 0; x < world::CHUNK_SIZE; x++) {
			for (int z = 0; z < world::CHUNK_SIZE; z++) {
				chunk->setBlock(x, 0, z, block::STONE);
				chunk->setBlock(x, 1, z, block::STONE);
				chunk->setBlock(x, 2, z, block::STONE);
				chunk->setBlock(x, 3, z, block::DIRT);
				chunk->setBlock(x, 4, z, block::DIRT);
				chunk->setBlock(x, 5, z, block::GRASS);
			}
		}

		mesh = gfx::ChunkMesh::createMesh(chunk);
	}

	void terminateRender() {
		delete shader;
		delete camera;
		delete window;
		delete texture;
		delete mesh;

		delete border_shader;
		delete border;

		glfwTerminate();
	}

}