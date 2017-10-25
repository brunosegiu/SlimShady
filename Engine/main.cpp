#include <iostream>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <ctime>
#include <windows.h>

#include "World.h"
#include "Camera.h"
#include "NormalMappedMesh.h"
#include "Mesh.h"
#include "Water.h"

using namespace std;

void init();
void initGL();
void draw(World &w);
void close();

SDL_Window* window = NULL;
SDL_GLContext context;

int WIDTH = 1280;
int HEIGHT = 720;

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
	Camera* cam = new Camera(WIDTH, HEIGHT, 45.0f, window);
	World* test = new World(cam);

	Entity* test_mesh, *test_normal_map;
	test_mesh = new Mesh("assets/models/boulder");
	//test_mesh->translate(glm::vec3(250.0f, 5.0f, 100.0f));
	test_normal_map = new NormalMappedMesh("assets/models/boulder");
	test_normal_map->translate(glm::vec3(-10.0f, 0.0f, 0.0f));
	test->meshes.push_back(test_mesh);
	test->meshes_nm.push_back(test_normal_map);

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
						exit = true;
					}
					else if (event.key.keysym.sym == SDLK_z) {
						wireframe = !wireframe;
						if (wireframe) {
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						}
						else {
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						}
					}
				}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					cam->in = !cam->in;
					if (cam->in)  {
						SDL_ShowCursor(SDL_DISABLE);
					}
					else
						SDL_ShowCursor(SDL_ENABLE);
				}
				break;
			}
		}
		draw(*test);
		double dif = frameTime - ((clock() - start) * (1000.0 / double(CLOCKS_PER_SEC)) );
		if (dif > 0) {
			Sleep(int(dif));
		}
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}
