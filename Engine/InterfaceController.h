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
		void showGraphics();
		void selectPath(int type);
	public:
		bool entities;
		bool path1, path2, path3, path4;
		bool addent;
		bool editing;
		bool graphics;
		bool save, load;
		SDL_Window* window;
		World* world;
};
