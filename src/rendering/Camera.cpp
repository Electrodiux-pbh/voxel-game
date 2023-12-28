#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace electrodiux::voxel::gfx {

	void Camera::updateProjectionMatrix() {
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	Camera::Camera(float fov, float aspect, float near, float far) {
		this->fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;

		updateProjectionMatrix();
	}

	glm::mat4 Camera::calculateViewMatrix()
	{
		view = glm::mat4(1.0f);

		view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		view = glm::translate(view, -position);

		return view;
	}

	glm::mat4 Camera::getProjectionMatrix() {
		return this->projection;
	}

	void Camera::setFov(float fov) {
		if(this->fov == fov) return;

		this->fov = fov;
		updateProjectionMatrix();
	}

	void Camera::setAspect(float aspect) {
		if(this->aspect == aspect) return;

		this->aspect = aspect;
		updateProjectionMatrix();
	}

	void Camera::setNear(float near) {
		if(this->near == near) return;

		this->near = near;
		updateProjectionMatrix();
	}

	void Camera::setFar(float far) {
		if(this->far == far) return;

		this->far = far;
		updateProjectionMatrix();
	}

	void Camera::setProjection(float fov, float aspect, float near, float far) {
		if(this->fov == fov && this->aspect == aspect && this->near == near && this->far == far) return;

		this->fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;
		updateProjectionMatrix();
	}

	float Camera::getFov() const {
		return this->fov;
	}

	float Camera::getAspect() const {
		return this->aspect;
	}

	float Camera::getNear() const {
		return this->near;
	}

	float Camera::getFar() const {
		return this->far;
	}

}