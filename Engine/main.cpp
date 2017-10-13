#include <iostream>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <ctime>
#include <windows.h>

#include "World.h"
#include "Camera.h"

using namespace std;

void init();
void initGL();
void draw(World &w, Camera &cam);
void close();

SDL_Window* window = NULL;
SDL_GLContext context;

int WIDTH = 1280;
int HEIGHT = 720;

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();
	initGL();
}

void initGL() {
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, (WIDTH / float(HEIGHT)), 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//OpenGL attribs
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);
	glEnable(GL_FLAT);
}

void draw(World &w, Camera &cam) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	GLfloat spec[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat dif[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat pos[] = { -1.0, 1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	cam.update();	
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
	World test;
	double frameTime = 1000.0f / 65.0f;

	Camera cam = Camera(WIDTH, HEIGHT, window);
	
	test.meshes.push_back(new Mesh("assets/models/stormtrooper"));
	
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
							glDisable(GL_LIGHTING);
						}
						else {
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							glEnable(GL_LIGHTING);
						}
					}
				}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					cam.in = !cam.in;
					if (cam.in) {
						SDL_ShowCursor(SDL_DISABLE);
					}
					else
						SDL_ShowCursor(SDL_ENABLE);
				}
				break;
			}
		}
		draw(test, cam);
		double dif = frameTime - ((clock() - start) * (1000.0 / double(CLOCKS_PER_SEC)) );
		if (dif > 0) {
			Sleep(int(dif));
		}
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}
