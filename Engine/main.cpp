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

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

using namespace std;

void init();
void initGL();
void draw(World &w);
void close();

// GUI functions

void showEntities(World &world);
void addEntity(World &world);
void selectPath(World &world, int type);

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

bool entities = false;
bool path1 = false;
bool path2 = false;
bool addent = false;

int main(int argc, char* argv[]) {
	init();

	ImGui_ImplSdlGL3_Init(window);

	bool exit = false;
	bool editing = false;
	
	bool wireframe = false;

	SDL_Event event;
	
	double frameTime = 1000.0f / 65.0f;
	Camera* cam = new Camera(WIDTH, HEIGHT, 45.0f, window);
	World* test = new World(cam);

	Model* mesh = new Mesh("assets/models/boulder");
	Model* mesh2 = new NormalMappedMesh("assets/models/boulder");
	test->addModel(mesh);
	test->addModel(mesh2);
	test->addEntity(mesh->name);
	test->addEntity(mesh2->name);

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
					editing = !editing;
					cam->in = !editing;

					if (!editing) {
						entities = false;
					}
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
				cam->in = !cam->in && !editing;
				if (cam->in) {
					SDL_ShowCursor(SDL_DISABLE);
				}
				else
					SDL_ShowCursor(SDL_ENABLE);
			}
			break;
			}
			if (editing)
				ImGui_ImplSdlGL3_ProcessEvent(&event);
		}

		if (editing) {
			ImGui_ImplSdlGL3_NewFrame(window);
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {
					}
					if (ImGui::MenuItem("Save")) {
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::BeginMenu("Add model")) {
						if (ImGui::MenuItem("Mesh")) {
							path1 = true;
						}
						if (ImGui::MenuItem("Normal mapped mesh")) {
							path2 = true;
						}
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Edit entities")) {
						entities = true;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			if (entities) {
				showEntities(*test);
			}
			if (addent) {
				addEntity(*test);
			}
			if (path1) {
				selectPath(*test, 1);
			}
			if (path2) {
				selectPath(*test, 2);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		

		draw(*test);
		if (editing)
			ImGui::Render();
		double dif = frameTime - ((clock() - start) * (1000.0 / double(CLOCKS_PER_SEC)) );
		if (dif > 0) {
			Sleep(int(dif));
		}
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}

void showEntities(World &world) {
	ImGui::Begin("Entities", &entities);
	for (unsigned int i = 0; i < world.meshes.size(); i++) {
		if (ImGui::CollapsingHeader( (world.meshes[i]->model->name + "-ent: " + to_string(i) ).c_str())) {
			ImGui::Text("Translate:");
			ImGui::SliderFloat("X ", &world.meshes[i]->acumulatedTranslate.x, -50.0f, 50.0f);
			ImGui::SliderFloat("Y ", &world.meshes[i]->acumulatedTranslate.y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z ", &world.meshes[i]->acumulatedTranslate.z, -50.0f, 50.0f);

			ImGui::Text("Rotate:");
			ImGui::SliderFloat("X  ", &world.meshes[i]->acumulatedRotate.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Y  ", &world.meshes[i]->acumulatedRotate.y, -5.0f, 5.0f);
			ImGui::SliderFloat("Z  ", &world.meshes[i]->acumulatedRotate.z, -5.0f, 5.0f);

			ImGui::Text("Scale:");
			ImGui::SliderFloat("X", &world.meshes[i]->acumulatedScale.x, 0.1f, 5.0f);
			ImGui::SliderFloat("Y", &world.meshes[i]->acumulatedScale.y, 0.1f, 5.0f);
			ImGui::SliderFloat("Z", &world.meshes[i]->acumulatedScale.z, 0.1f, 5.0f);
		}
	}
	for (unsigned int i = 0; i < world.meshes_nm.size(); i++) {
		if (ImGui::CollapsingHeader((world.meshes_nm[i]->model->name + "-ent: " + to_string(i)).c_str())) {
			ImGui::Text("Translate:");
			ImGui::SliderFloat("X ", &world.meshes_nm[i]->acumulatedTranslate.x, -50.0f, 50.0f);
			ImGui::SliderFloat("Y ", &world.meshes_nm[i]->acumulatedTranslate.y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z ", &world.meshes_nm[i]->acumulatedTranslate.z, -50.0f, 50.0f);

			ImGui::Text("Rotate:");
			ImGui::SliderFloat("X  ", &world.meshes_nm[i]->acumulatedRotate.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Y  ", &world.meshes_nm[i]->acumulatedRotate.y, -5.0f, 5.0f);
			ImGui::SliderFloat("Z  ", &world.meshes_nm[i]->acumulatedRotate.z, -5.0f, 5.0f);

			ImGui::Text("Scale:");
			ImGui::SliderFloat("X", &world.meshes_nm[i]->acumulatedScale.x, 0.1f, 5.0f);
			ImGui::SliderFloat("Y", &world.meshes_nm[i]->acumulatedScale.y, 0.1f, 5.0f);
			ImGui::SliderFloat("Z", &world.meshes_nm[i]->acumulatedScale.z, 0.1f, 5.0f);
		}
	}
	if (ImGui::Button("Add entity")) {
		addent = true;
	}
	ImGui::End();
}

void addEntity(World &world) {
	ImGui::Begin("Add entity", &addent);
	for (auto const &ent : world.models){
		if (ImGui::Selectable(ent.first.c_str())){
			world.addEntity(ent.first);
			addent = false;
		}
	}
	ImGui::End();
}

char input[160] = "Write the file path here, you dummy";

void selectPath(World &world, int type) {
	if (path1)
		ImGui::Begin("Select path", &path1);
	else
		ImGui::Begin("Select path", &path2);
	ImGui::InputText("Path (sorry)", input, 160);
	if (ImGui::Button("Accept")) {
		if (type == 1) {
			try {
				world.addModel(new Mesh(input));
			}
			catch (exception &e) {
			}
			path1 = false;
		}
		else if (type == 2) {
			try {
				world.addModel(new NormalMappedMesh(input));
			}
			catch (exception &e) {
			}
			path2 = false;
		}
	}
	ImGui::End();
}
