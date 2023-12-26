#include "graphics_libs.h"
#include "Window.h"

#include <iostream>

namespace electrodiux::voxel::gfx {

	Window::Window(int width, int height, const char* title) {
		glfwWindowHint(GLFW_SAMPLES, GLFW_TRUE);

		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (!window) {
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		this->glfw_window = window;
	}

	Window::~Window() {
		glfwDestroyWindow(this->glfw_window);
	}

	void Window::setWindowAttrib(int attrib, int value) {
		glfwSetWindowAttrib(this->glfw_window, attrib, value);
	}

	int Window::getWindowAttrib(int attrib) const {
		return glfwGetWindowAttrib(this->glfw_window, attrib);
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(this->glfw_window);
	}

	void Window::swapBuffers() {
		glfwSwapBuffers(this->glfw_window);
	}

	void Window::setSizeCallback(WindowSizefun callback) {
		glfwSetWindowSizeCallback(this->glfw_window, callback);
	}

	void Window::setKeyCallback(WindowKeyfun callback) {
		glfwSetKeyCallback(this->glfw_window, callback);
	}

	void Window::setInputMode(int mode, int value) {
		glfwSetInputMode(this->glfw_window, mode, value);
	}

	void Window::close() {
		glfwSetWindowShouldClose(this->glfw_window, GLFW_TRUE);
	}

	GLFWwindow* Window::getGLFWWindow() {
		return this->glfw_window;
	}

}