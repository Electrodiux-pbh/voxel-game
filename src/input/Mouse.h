#pragma once

namespace electrodiux::input {

	constexpr int MOUSE_BUTTONS = 8;

	constexpr int MOUSE_PRESS = 1;
	constexpr int MOUSE_RELEASE = 0;

	constexpr int MOUSE_BUTTON_LEFT = 0;
	constexpr int MOUSE_BUTTON_RIGHT = 1;
	constexpr int MOUSE_BUTTON_MIDDLE = 2;

	class Mouse {
	
		private:
		double x, y;
		double last_x, last_y;
		double scroll_x, scroll_y;

		bool draggin;

		bool buttons_pressed[MOUSE_BUTTONS];
		bool buttons_clicked[MOUSE_BUTTONS];

		public:
		void positionCallback(double x, double y);
		void scrollCallback(double x, double y);
		void buttonCallback(int button, int action, int mods);

		void update();

		double getX();
		double getY();

		double getDX();
		double getDY();

		double getScrollX();
		double getScrollY();

		bool isDragging();

		bool isButtonPressed(int button);
		bool isButtonClicked(int button);
	};

}