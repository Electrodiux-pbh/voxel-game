#pragma once

namespace electrodiux::input {

	constexpr int KEY_BUFFER_SIZE = 512;

	constexpr int KEY_RELEASE = 0;
	constexpr int KEY_PRESS = 1;

	class Keyboard {

		private:
		bool keys_pressed[KEY_BUFFER_SIZE];
		bool keys_typed[KEY_BUFFER_SIZE];

		public:
		void keyCallback(int key, int scancode, int action, int mods);

		void update();

		bool isKeyPressed(int key);
		bool isKeyTyped(int key);

	};

}