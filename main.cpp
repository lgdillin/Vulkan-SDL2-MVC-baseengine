

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "View.hpp"
#include "Controller.hpp"
#include "VkInit.hpp"


std::chrono::steady_clock::time_point frameStart, frameEnd;
int frameCount = 0;
static void calculateFPS(View *_view) {
	frameEnd = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

	if (duration >= 1000) {
		int fps = frameCount * 1000 / duration;
		//std::cout << "FPS: " << fps << std::endl;

		std::string title = "FPS Counter - FPS: " + std::to_string(fps);
		SDL_SetWindowTitle(_view->getWindow(), title.c_str());

		frameStart = std::chrono::steady_clock::now();
		frameCount = 0;
	} else {
		frameCount++;
	}
}

int main(int argc, char *argv[]) {
	// Init MVC
	Game game;

	Controller controller(game);


	View view(game, controller);
	view.initialize();

	game.initialize();
	//instance::createInstance(view.getWindow(), *view.getInstance());

	bool quit = false;
	Uint32 frameStart;
	const int targetFramerate = 40;
	const int frameDelay = 1000 / targetFramerate;
	while (!quit) {
		frameStart = SDL_GetTicks();

		// collect user input
		controller.update();

		// update data model/compute physics
		game.update();

		// update the screen
		view.repaint();

		calculateFPS(&view);
	}

	return 0;
}