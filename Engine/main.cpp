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

int WIDTH = 800;
int HEIGHT = 600;

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
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, (WIDTH / float(HEIGHT)), 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//OpenGL attribs
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);
}

void draw(World &w) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	w.dummyDraw(); // Using basic shading
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
	World test = World(cam);

	test.worldEntities.push_back(new Entity(new Mesh("assets/models/stormtrooper")));

	/*float coords[] = {
		0.0f,0.0f,0.0f,
		10.0f,0.0f,0.0f,
		10.0f,0.0f,-10.0f,
		0.0f,0.0f,-10.0f
	};
	vector<float> positions (coords, coords + sizeof(coords) / sizeof(coords[0]));
	unsigned int indices[] = { 0,1,2,2,3,0 };
	vector<unsigned int> index(indices, indices + sizeof(indices) / sizeof(indices[0]));*/
	vector<float> positions;
	vector<unsigned int> index;
	for (unsigned int i = 0; i <= 10; i++) {
		for (unsigned int j = 0; j <= 10; j++) {
			positions.push_back((float)-5+j);
			positions.push_back(0.0f);
			positions.push_back((float)-5+i);
		}
	}
	for (unsigned int i = 0; i <= 9; i++) {
		for (unsigned int j = 0; j <= 9; j++) {
			index.push_back(i * 11 + j);
			index.push_back(i * 11 + j + 1);
			index.push_back(i * 11 + j + 11 +1);

			index.push_back(i * 11 + j + 11 + 1);
			index.push_back(i * 11 + j + 11);
			index.push_back(i * 11 + j);
		}
	}
	test.meshes.push_back(new FreeMesh(positions,index));
	
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
					cam->in = !cam->in;
					if (cam->in) {
						SDL_ShowCursor(SDL_DISABLE);
					}
					else
						SDL_ShowCursor(SDL_ENABLE);
				}
				break;
			}
		}
		draw(test);
		double dif = frameTime - ((clock() - start) * (1000.0 / double(CLOCKS_PER_SEC)) );
		if (dif > 0) {
			Sleep(int(dif));
		}
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}
