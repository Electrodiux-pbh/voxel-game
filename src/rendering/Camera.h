#pragma once

#include <glm/glm.hpp>

namespace electrodiux::voxel::gfx {

	class Camera {
	
		private:
		glm::mat4 projection, view;

		public:
		glm::vec3 position, rotation;

		private:
		float fov, aspect, near, far;

		void updateProjectionMatrix();

		public:
		Camera(float fov, float aspect, float near, float far);

		glm::mat4 calculateViewMatrix();

		glm::mat4 getProjectionMatrix();

		void setFov(float fov);
		void setAspect(float aspect);
		void setNear(float near);
		void setFar(float far);

		void setProjection(float fov, float aspect, float near, float far);

		float getFov() const;
		float getAspect() const;
		float getNear() const;
		float getFar() const;

	};

}