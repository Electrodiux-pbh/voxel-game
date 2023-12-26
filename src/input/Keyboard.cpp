#include "Keyboard.h"

#include <string>

namespace electrodiux::input {

	void Keyboard::keyCallback(int key, int scancode, int action, int mods) {
		if (key < 0 || key >= KEY_BUFFER_SIZE) return;

		if (action == KEY_PRESS) {
			keys_pressed[key] = true;
			keys_typed[key] = true;
		}
		else if (action == KEY_RELEASE) {
			keys_pressed[key] = false;
			keys_typed[key] = false;
		}
	}

	void Keyboard::update() {
		memset(keys_typed, 0, KEY_BUFFER_SIZE * sizeof(bool));
	}

	bool Keyboard::isKeyPressed(int key) {
		if(key < 0 || key >= KEY_BUFFER_SIZE) return false;
		return keys_pressed[key];
	}

	bool Keyboard::isKeyTyped(int key) {
		if(key < 0 || key >= KEY_BUFFER_SIZE) return false;
		return keys_typed[key];
	}

}