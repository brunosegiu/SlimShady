#include <iostream>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <ctime>
#include <windows.h>

#include "World.h"
#include "Camera.h"
#include "NormalMappedMesh.h"
#include "MeshInstanced.h"
#include "Mesh.h"
#include "Water.h"
#include "Animation.h"
#include "InterfaceController.h"

using namespace std;

void init();
void initGL();
void draw(World &w);
void close();

SDL_Window* window = NULL;
SDL_GLContext context;

int WIDTH = 1600;
int HEIGHT = 900;

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow("SlimShady", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();
	initGL();
}

void initGL() {
	glClearColor(0.0f, 0.7f, 1.0f, 1.0f);

	//OpenGL attribs
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void draw(World &w) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	w.draw();
}

void close() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}



int main(int argc, char* argv[]) {
	init();

	bool exit = false;

	bool wireframe = false;

	SDL_Event event;
	
	double frameTime = 1000.0f / 65.0f;

	World* test = World::load("test3",window,WIDTH, HEIGHT);
	
	InterfaceController controller = InterfaceController(window, test);
	std::clock_t start;
	while (!exit) {
		start = clock();
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				exit = true;
				break;
			case SDL_KEYDOWN: {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					controller.editing = !controller.editing;
					controller.world->cam->in = !controller.editing;
				}
				else if (event.key.keysym.sym == SDLK_l) {
					if (test->sun->stop) test->sun->stop = false;
					else test->sun->stop = true;
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN: {
				controller.world->cam->in = !controller.world->cam->in && !controller.editing;
				if (controller.world->cam->in) {
					SDL_ShowCursor(SDL_DISABLE);
				}
				else
					SDL_ShowCursor(SDL_ENABLE);
			}
			break;
			}
			controller.processEvent(event);
		}
		controller.update();
		test = controller.world;

		glClear(GL_COLOR_BUFFER_BIT);
		draw(*test);
		controller.draw();
		double dif = frameTime - ((clock() - start) * (1000.0 / double(CLOCKS_PER_SEC)) );
		if (dif > 0) {
			Sleep(int(dif));
		}
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}
