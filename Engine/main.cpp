#include <iostream>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <ctime>

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
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
	glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void draw(World &w, Camera &cam) {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	cam.update();

	glBegin(GL_TRIANGLES);
	glVertex3f(-1.5f, 1.0f, -6.0f);
	glVertex3f(-2.5f, -1.0f, -6.0f);
	glVertex3f(-0.5f, -1.0f, -6.0f);
	glEnd();
	
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
	SDL_Event event;
	World test;
	
	

	Camera cam = Camera(WIDTH, HEIGHT, window);
	
	#ifndef EXPERIMENTAL_MESH
	Mesh m;
	m.load("susana");
	test.meshes.push_back(/*Mesh("susana")*/ m);
	#else
	test.meshes.push_back(Mesh("susana"));
	#endif
	

	std::clock_t start = clock();
	while (!exit) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					exit = true;
					break;
				case SDL_KEYDOWN: {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						exit = true;
					}
				}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					cam.in = !cam.in;
					if (cam.in)
						SDL_ShowCursor(SDL_DISABLE);
					else
						SDL_ShowCursor(SDL_ENABLE);
				}
				break;
			}
		}
		draw(test, cam);
		SDL_GL_SwapWindow(window);
		//printf("%f \n", ((clock() - start) / double(CLOCKS_PER_SEC))); //Frame time
		start = clock();
	}
	close();
	return 0;
}
