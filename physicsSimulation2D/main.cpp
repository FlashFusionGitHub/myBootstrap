#include "physicsSimulation2DApp.h"

int main() {
	
	// allocation
	auto app = new physicsSimulation2DApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}