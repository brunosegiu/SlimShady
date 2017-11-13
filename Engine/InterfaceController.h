#pragma once

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "World.h"

class InterfaceController {
	public:
		InterfaceController(SDL_Window* window, World* world);
		void processEvent(SDL_Event &event);
		void update();
		void draw();
		void showEntities();
		void addEntity();
		void selectPath(int type);
	public:
		bool entities;
		bool path1;
		bool path2;
		bool addent;
		bool editing;
		SDL_Window* window;
		World* world;
};
