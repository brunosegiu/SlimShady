#include "InterfaceController.h"

#include "Entity.h"
#include "NormalMappedMesh.h"
#include "Mesh.h"


void InterfaceController::showEntities() {
	ImGui::Begin("Entities", &entities);
	for (unsigned int i = 0; i < world->meshes.size(); i++) {
		if (ImGui::CollapsingHeader((world->meshes[i]->model->name + "-ent: " + to_string(i)).c_str())) {
			ImGui::Text("Translate:");
			ImGui::SliderFloat("X ", &world->meshes[i]->acumulatedTranslate.x, -500.0f, 500.0f);
			ImGui::SliderFloat("Y ", &world->meshes[i]->acumulatedTranslate.y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z ", &world->meshes[i]->acumulatedTranslate.z, -500.0f, 500.0f);

			ImGui::Text("Rotate:");
			ImGui::SliderFloat("X  ", &world->meshes[i]->acumulatedRotate.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Y  ", &world->meshes[i]->acumulatedRotate.y, -5.0f, 5.0f);
			ImGui::SliderFloat("Z  ", &world->meshes[i]->acumulatedRotate.z, -5.0f, 5.0f);

			ImGui::Text("Scale:");
			ImGui::SliderFloat("X", &world->meshes[i]->acumulatedScale.x, 0.1f, 50.0f);
			ImGui::SliderFloat("Y", &world->meshes[i]->acumulatedScale.y, 0.1f, 50.0f);
			ImGui::SliderFloat("Z", &world->meshes[i]->acumulatedScale.z, 0.1f, 50.0f);

			if (ImGui::Button("Remove")) {
				delete world->meshes[i];
				world->meshes.erase(world->meshes.begin() + i);
			}
		}
	}
	for (unsigned int i = 0; i < world->meshes_nm.size(); i++) {
		if (ImGui::CollapsingHeader((world->meshes_nm[i]->model->name + "-ent: " + to_string(i)).c_str())) {
			ImGui::Text("Translate:");
			ImGui::SliderFloat("X ", &world->meshes_nm[i]->acumulatedTranslate.x, -500.0f, 500.0f);
			ImGui::SliderFloat("Y ", &world->meshes_nm[i]->acumulatedTranslate.y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z ", &world->meshes_nm[i]->acumulatedTranslate.z, -500.0f, 500.0f);

			ImGui::Text("Rotate:");
			ImGui::SliderFloat("X  ", &world->meshes_nm[i]->acumulatedRotate.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Y  ", &world->meshes_nm[i]->acumulatedRotate.y, -5.0f, 5.0f);
			ImGui::SliderFloat("Z  ", &world->meshes_nm[i]->acumulatedRotate.z, -5.0f, 5.0f);

			ImGui::Text("Scale:");
			ImGui::SliderFloat("X", &world->meshes_nm[i]->acumulatedScale.x, 0.1f, 50.0f);
			ImGui::SliderFloat("Y", &world->meshes_nm[i]->acumulatedScale.y, 0.1f, 50.0f);
			ImGui::SliderFloat("Z", &world->meshes_nm[i]->acumulatedScale.z, 0.1f, 50.0f);

			if (ImGui::Button("Remove")) {
				delete world->meshes_nm[i];
				world->meshes_nm.erase(world->meshes_nm.begin() + i);
			}
		}
	}
	if (ImGui::CollapsingHeader("Water-ent: ")) {
		ImGui::Text("Translate:");
		ImGui::SliderFloat("X ", &world->water->acumulatedTranslate.x, -500.0f, 500.0f);
		ImGui::SliderFloat("Y ", &world->water->acumulatedTranslate.y, -50.0f, 50.0f);
		ImGui::SliderFloat("Z ", &world->water->acumulatedTranslate.z, -500.0f, 500.0f);

		ImGui::Text("Rotate:");
		ImGui::SliderFloat("X  ", &world->water->acumulatedRotate.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Y  ", &world->water->acumulatedRotate.y, -5.0f, 5.0f);
		ImGui::SliderFloat("Z  ", &world->water->acumulatedRotate.z, -5.0f, 5.0f);

		ImGui::Text("Scale:");
		ImGui::SliderFloat("X", &world->water->acumulatedScale.x, 0.1f, 50.0f);
		ImGui::SliderFloat("Y", &world->water->acumulatedScale.y, 0.1f, 50.0f);
		ImGui::SliderFloat("Z", &world->water->acumulatedScale.z, 0.1f, 50.0f);
	}
	for (unsigned int i = 0; i < world->terrains.size(); i++) {
		if (ImGui::CollapsingHeader(("Terrain-ent: " + to_string(i)).c_str())) {
			ImGui::Text("Translate:");
			ImGui::SliderFloat("X ", &world->terrains[i]->acumulatedTranslate.x, -500.0f, 500.0f);
			ImGui::SliderFloat("Y ", &world->terrains[i]->acumulatedTranslate.y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z ", &world->terrains[i]->acumulatedTranslate.z, -500.0f, 500.0f);

			ImGui::Text("Rotate:");
			ImGui::SliderFloat("X  ", &world->terrains[i]->acumulatedRotate.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Y  ", &world->terrains[i]->acumulatedRotate.y, -5.0f, 5.0f);
			ImGui::SliderFloat("Z  ", &world->terrains[i]->acumulatedRotate.z, -5.0f, 5.0f);

			ImGui::Text("Scale:");
			ImGui::SliderFloat("X", &world->terrains[i]->acumulatedScale.x, 0.1f, 50.0f);
			ImGui::SliderFloat("Y", &world->terrains[i]->acumulatedScale.y, 0.1f, 50.0f);
			ImGui::SliderFloat("Z", &world->terrains[i]->acumulatedScale.z, 0.1f, 50.0f);
		}
	}
	if (ImGui::Button("Add entity")) {
		addent = true;
	}
	ImGui::End();
}

void InterfaceController::addEntity() {
	ImGui::Begin("Add entity", &addent);
	for (auto const &ent : world->models) {
		if (ImGui::Selectable(ent.first.c_str())) {
			world->addEntity(ent.first);
			addent = false;
		}
	}
	ImGui::End();
}

char input[160] = "";

void InterfaceController::selectPath(int type) {
	if (path1)
		ImGui::Begin("Select path", &path1);
	else if (path2)
		ImGui::Begin("Select path", &path2);
	else if (path3)
		ImGui::Begin("Select path", &path3);
	else if (path4)
		ImGui::Begin("Select path", &path4);
	ImGui::InputText("Path (sorry)", input, 160);
	if (ImGui::Button("Accept")) {
		if (type == 1) {
			try {
				world->addModel(new Mesh(input));
			}
			catch (exception &e) {
			}
			path1 = false;
		}
		else if (type == 2) {
			try {
				world->addModel(new NormalMappedMesh(input));
			}
			catch (exception &e) {
			}
			path2 = false;
		}
		else if (type == 3) {
			world->save(input);
		}
		else if (type == 4) {
			int w, h;
			SDL_GetWindowSize(window, &w, &h);
			delete world;
			world = World::load(input, window, w, h);
		}
	}
	ImGui::End();
}

InterfaceController::InterfaceController(SDL_Window* window, World* world) {
	entities = false;
	path1 = false;
	path2 = false;
	addent = false;
	editing = false;
	graphics = false;
	ImGui_ImplSdlGL3_Init(window);
	this->window = window;
	this->world = world;
}

void InterfaceController::update() {
	if (editing) {
		ImGui_ImplSdlGL3_NewFrame(window);
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open")) {
					path4 = true;
				}
				if (ImGui::MenuItem("Save")) {
					path3 = true;
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
			if (ImGui::BeginMenu("Graphics settings")) {
				graphics = true;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (entities) {
			showEntities();
		}
		if (addent) {
			addEntity();
		}
		if (path1) {
			selectPath(1);
		}
		if (path2) {
			selectPath(2);
		}
		if (path3)
			selectPath(3);
		if (path4)
			selectPath(4);
		if (graphics) {
			showGraphics();
		}
	}
}

void InterfaceController::draw() {
	if (editing)
		ImGui::Render();
}

void InterfaceController::showGraphics() {
	ImGui::Begin("Graphics settings", &graphics);
	ImGui::LabelText("-.1", "Color correction: ");
	ImGui::SliderFloat("Gamma", &world->gamma, 0.0f, 5.0f);
	ImGui::SliderFloat("Brightness", &world->brightness, 0.0f, 1.0f);
	ImGui::SliderFloat("Contrast", &world->contrast, 0.0f, 5.0f);
	ImGui::LabelText("-.2", "Post-processing effects: ");
	ImGui::SliderFloat("Vignette", &world->vignette, 0.0f, 5.0f);
	ImGui::SliderFloat("Fog Exponent", &world->fogFactor, 0.0f, 10.0f);
	ImGui::SliderFloat("Blur factor", &world->blurFactor, 0.0f, 50.0f);
	ImGui::Checkbox("FXAA", &world->filters[0].first);
	ImGui::Checkbox("DOF", &world->filters[2].first);
	ImGui::LabelText("-.3", "Time parameters");
	ImGui::Checkbox("Stop", &this->world->sun->stop);
	ImGui::End();
}

void InterfaceController::processEvent(SDL_Event &event) {
	ImGui_ImplSdlGL3_ProcessEvent(&event);
}