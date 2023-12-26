#include "rendering/graphics_libs.h"
#include "rendering/render.h"
#include "input/input.h"

#include <iostream>

using namespace electrodiux;

int main() {
	int status = initialize();
	if (status != 0) {
		std::cout << "Failed to initialize" << std::endl;
		return status;
	}

	loadResources();
	renderLoop();
	terminateRender();

	delete input::mouse;
	delete input::keyboard;

	return 0;
}
