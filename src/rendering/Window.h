#pragma once

struct GLFWwindow;
typedef void (*WindowKeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void (*WindowSizefun)(GLFWwindow* window, int width, int height);

namespace electrodiux::voxel::gfx {

	constexpr int CURSOR = 208897;
	
	constexpr int CURSOR_NORMAL = 212993;
	constexpr int CURSOR_HIDDEN = 212994;
	constexpr int CURSOR_DISABLED = 212995;

	class Window {
		private:
		GLFWwindow* glfw_window;

		public:
		Window(int width, int height, const char* title);
		~Window();

		void setWindowAttrib(int attrib, int value);
		int getWindowAttrib(int attrib) const;

		bool shouldClose() const;

		void swapBuffers();

		void setSizeCallback(WindowSizefun callback);

		void setKeyCallback(WindowKeyfun callback);

		void setInputMode(int mode, int value);

		void close();

		GLFWwindow* getGLFWWindow();
	};

}