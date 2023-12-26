#include "Mouse.h"

namespace electrodiux::input {

	void Mouse::positionCallback(double x, double y) {
		last_x = this->x;
		last_y = this->y;
		this->x = x;
		this->y = y;
		draggin = buttons_pressed[0] || buttons_pressed[1] || buttons_pressed[2];
	}

	void Mouse::scrollCallback(double x, double y) {
		this->scroll_x = x;
		this->scroll_y = y;
	}

	void Mouse::buttonCallback(int button, int action, int mods) {
		if(button < 0 || button >= MOUSE_BUTTONS) return;

		if (action == MOUSE_PRESS) {
			buttons_pressed[button] = true;
			buttons_clicked[button] = true;
		} else if (action == MOUSE_RELEASE) {
			buttons_pressed[button] = false;
			buttons_clicked[button] = false;
			this->draggin = false;
		}
	}

	void Mouse::update() {
		scroll_x = 0;
		scroll_y = 0;
		last_x = x;
		last_y = y;
		for (int i = 0; i < MOUSE_BUTTONS; i++) {
			buttons_clicked[i] = false;
		}
	}

	double Mouse::getX() {
		return this->x;
	}

	double Mouse::getY() {
		return this->y;
	}

	double Mouse::getDX() {
		return this->x - this->last_x;
	}

	double Mouse::getDY() {
		return this->y - this->last_y;
	}

	double Mouse::getScrollX() {
		return this->scroll_x;
	}

	double Mouse::getScrollY() {
		return this->scroll_y;
	}

	bool Mouse::isDragging() {
		return this->draggin;
	}

	bool Mouse::isButtonPressed(int button) {
		if(button < 0 || button >= MOUSE_BUTTONS)
			return false;
		return this->buttons_pressed[button];
	}

	bool Mouse::isButtonClicked(int button) {
		if(button < 0 || button >= MOUSE_BUTTONS)
			return false;
		return this->buttons_clicked[button];
	}

}