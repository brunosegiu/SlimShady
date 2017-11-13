#include "World.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "tinyxml2.h"
#include "Mesh.h"
#include "NormalMappedMesh.h"
#include "MeshInstanced.h"

World::World(Camera* cam) {
	this->cam = cam;
	//Shaders
	this->basic = new ShaderProgram("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
	this->basicNM = new ShaderProgram("assets/shaders/mesh_normalmap.vert", "assets/shaders/mesh_normalmap.frag");
	this->basicInst = new ShaderProgram("assets/shaders/meshInst.vert", "assets/shaders/meshInst.frag");

	addModel(new Terrain("assets/textures/valley.png", 35.0f, 20, 20));
	addModel(new Water(300, 300));
	//addModel(new Skybox(600));
	this->terrains.push_back(new Entity(models["Terrain"], this));
	this->water = new Entity(models["Water"], this);
	this->water->scale(glm::vec3(2.0f, 2.0f, 2.0f));
	//this->water->translate(glm::vec3(0.0f, -30.0f, 0.0f));
	this->terrains[0]->translate(glm::vec3(-256, 30.0f, -256));
	this->terrains[0]->scale(glm::vec3(1.5, 1.0f, 1.5f));

	this->lastDraw = clock();

	this->sun = new Sun(lastDraw);
	this->sky = new Skybox(1000);
}

void World::draw() {
	// Update drawing timer
	float elapsed = (clock() - this->lastDraw) / double(CLOCKS_PER_SEC);
	this->lastDraw = clock();

	//Update Sun
	this->sun->updateLight(lastDraw);

	// Update Camera view
	this->cam->update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render meshes
	if (meshes.size() > 0) {
		this->basic->bind();
		for (unsigned int i = 0; i < meshes.size(); i++) {
			this->meshes[i]->draw(basic->getId());
		}
		this->basic->unbind();
	}
	
	//Render normal mapped meshes
	if (meshes_nm.size() > 0) {
		this->basicNM->bind();
		for (unsigned int i = 0; i < meshes_nm.size(); i++) {
			this->meshes_nm[i]->draw(basicNM->getId());
		}
		this->basicNM->unbind();
	}
	//Render instanced meshes
	if (meshes_inst.size() > 0) {
		this->basicInst->bind();
		for (unsigned int i = 0; i < meshes_inst.size(); i++) {
			this->meshes_inst[i]->draw(basicInst->getId());
		}
		this->basicInst->unbind();
	}
	

	//Render water
	Water* w = dynamic_cast<Water*>(this->water->model);
	w->mvp = this->cam->modelViewProjectionMatrix * water->modelMatrix;
	w->lastDraw = this->lastDraw;
	w->camPos = this->cam->pos;
	w->lightDir = sun->light->dir;
	w->lightColor = sun->light->color;
	w->intensity = sun->intensity;
	w->moonDir = sun->moon->dir;
	w->moonColor = sun->moon->color;
	w->mIntensity = sun->mIntensity;
	w->draw(0);

	//Render terrain
	for (unsigned int i = 0; i < terrains.size(); i++) {
		terrains[i]->draw(0);
	}

	//Render Skybox
	sky->mvp = this->cam->modelViewProjectionMatrix;
	sky->lightColor = sun->light->color;
	sky->intensity = sun->intensity;
	sky->moonColor = sun->moon->color;
	sky->mIntensity = sun->mIntensity;
	sky->draw(0);
	
	
	
}

void World::addModel(Model* model) {
	string name = model->name;
	while (models.count(name) > 0) {
		name += " bis";
	}
	model->name = name;
	this->models[model->name] = model;
}

void World::addEntity(string name) {
	if (this->models.count(name) > 0) {
		Model* model = this->models[name];
		Entity* ent = new Entity(model, this);
		if (dynamic_cast<NormalMappedMesh*>(model)) {
			this->meshes_nm.push_back(ent);
		}
		else if (dynamic_cast<MeshInstanced*>(model)) {
			this->meshes_inst.push_back(ent);
		}
		else if (dynamic_cast<Mesh*>(model)) {
			this->meshes.push_back(ent);
		}
		else if (dynamic_cast<Terrain*>(model)) {
			this->terrains.push_back(ent);
		}
	}
}

void World::save(string path) {
	/*using namespace tinyxml2;
	XMLDocument xmlDoc;

	XMLNode * pRoot = xmlDoc.NewElement("root");
	xmlDoc.InsertFirstChild(pRoot);

	XMLElement* pcam = xmlDoc.NewElement("camera");
	glm::vec3 position = this->cam->pos;
	pcam->SetAttribute("posx", position.x);
	pcam->SetAttribute("posy", position.y);
	pcam->SetAttribute("posz", position.z);

	glm::vec3 refer = this->cam->ref;
	pcam->SetAttribute("rx", refer.x);
	pcam->SetAttribute("ry", refer.y);
	pcam->SetAttribute("rz", refer.z);

	glm::vec3 up = this->cam->up;
	pcam->SetAttribute("upx", up.x);
	pcam->SetAttribute("upy", up.y);
	pcam->SetAttribute("upz", up.z);

	pcam->SetAttribute("fov", this->cam->fov);
	pcam->SetAttribute("sens", this->cam->sensitivity);
	pcam->SetAttribute("speed", this->cam->moveSpeed);

	pRoot->InsertEndChild(pcam);

	XMLElement * pmeshes = xmlDoc.NewElement("meshes");
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		Mesh* mesh = dynamic_cast<Mesh*>(meshes[i]);
		XMLElement * pmesh = xmlDoc.NewElement("item");
		pmesh->SetAttribute("path", mesh->path.c_str());
		pmeshes->InsertEndChild(pmesh);
	}
	pRoot->InsertEndChild(pmeshes);

	XMLElement * pnmmeshes = xmlDoc.NewElement("nmmeshes");
	for (unsigned int i = 0; i < this->meshes_nm.size(); i++) {
		NormalMappedMesh* mesh = dynamic_cast<NormalMappedMesh*>(meshes_nm[i]);
		XMLElement * pnmmesh = xmlDoc.NewElement("item");
		pnmmesh->SetAttribute("path", mesh->path.c_str());
		pnmmeshes->InsertEndChild(pnmmesh);
	}
	pRoot->InsertEndChild(pnmmeshes);

	XMLElement * pterrain = xmlDoc.NewElement("terrain");
	pterrain->SetAttribute("maxHeight", this->terrain->maxHeight);
	pterrain->SetAttribute("path", this->terrain->path.c_str());
	pterrain->SetAttribute("tilesX", this->terrain->tilesX);
	pterrain->SetAttribute("tilesY", this->terrain->tilesY);
	pRoot->InsertEndChild(pterrain);

	XMLElement * pdirectionallights = xmlDoc.NewElement("directionallights");
	for (unsigned int i = 0; i < this->dirLights.size(); i++) {
		XMLElement * plight = xmlDoc.NewElement("item");
		plight->SetAttribute("dirx", dirLights[i]->dir.x);
		plight->SetAttribute("diry", dirLights[i]->dir.y);
		plight->SetAttribute("dirz", dirLights[i]->dir.z);
		plight->SetAttribute("colorr", dirLights[i]->color.x);
		plight->SetAttribute("colorg", dirLights[i]->color.y);
		plight->SetAttribute("colorb", dirLights[i]->color.z);
		pdirectionallights->InsertEndChild(plight);
	}
	pRoot->InsertEndChild(pdirectionallights);
	xmlDoc.SaveFile(path.c_str());*/
}

World* World::load(string path, SDL_Window* win, float width, float height) {
	/*using namespace tinyxml2;
	XMLDocument xmlDoc;
	xmlDoc.LoadFile(path.c_str());

	XMLNode * pRoot = xmlDoc.FirstChild();

	XMLElement* pcam = pRoot->FirstChildElement("camera");
	float x, y, z;
	pcam->QueryFloatAttribute("posx", &x);
	pcam->QueryFloatAttribute("posy", &y);
	pcam->QueryFloatAttribute("posz", &z);

	float rx, ry, rz;
	pcam->QueryFloatAttribute("rx", &rx);
	pcam->QueryFloatAttribute("ry", &ry);
	pcam->QueryFloatAttribute("rz", &rz);

	float upx, upy, upz;
	pcam->QueryFloatAttribute("upx", &upx);
	pcam->QueryFloatAttribute("upy", &upy);
	pcam->QueryFloatAttribute("upz", &upz);

	float fov, sensitivity, speed;
	pcam->QueryFloatAttribute("fov", &fov);
	pcam->QueryFloatAttribute("sens", &sensitivity);
	pcam->QueryFloatAttribute("speed", &speed);

	Camera* cam = new Camera(glm::vec3(x, y, z), glm::vec3(rx, ry, rz), glm::vec3(upx, upy, upz), fov, sensitivity, speed, win, width, height);
	World* world = new World();
	world->cam = cam;

	XMLElement * pmeshes = pRoot->FirstChildElement("meshes");
	for (XMLElement* pmesh = pmeshes->FirstChildElement("item"); pmesh != NULL; pmesh = pmesh->NextSiblingElement("item")) {
		string path = std::string(pmesh->Attribute("path"));
		Mesh* m = new Mesh(path);
		world->meshes.push_back(m);
	}

	XMLElement * pnmmeshes = pRoot->FirstChildElement("nmmeshes");
	for (XMLElement* pmesh = pmeshes->FirstChildElement("item"); pmesh != NULL; pmesh = pmesh->NextSiblingElement("item")) {
		string path = std::string(pmesh->Attribute("path"));
		NormalMappedMesh* m = new NormalMappedMesh(path);
		world->meshes_nm.push_back(m);
	}

	XMLElement * pterrain = pRoot->FirstChildElement("terrain");
	float maxHeight;
	int tilesX, tilesY;
	pterrain->QueryFloatAttribute("maxHeight", &maxHeight);
	pterrain->QueryIntAttribute("tilesX", &tilesX);
	pterrain->QueryIntAttribute("tilesY", &tilesY);
	string hmpath = std::string(pterrain->Attribute("path"));
	world->terrain = new Terrain(hmpath, maxHeight, tilesX, tilesY);

	XMLElement * pdirlights = pRoot->FirstChildElement("directionallights");
	for (XMLElement* plight = pdirlights->FirstChildElement("item"); plight != NULL; plight = plight->NextSiblingElement("item")) {
		float dirx, diry, dirz;
		float colorr, colorg, colorb;
		plight->QueryFloatAttribute("dirx", &dirx);
		plight->QueryFloatAttribute("diry", &diry);
		plight->QueryFloatAttribute("dirz", &dirz);
		plight->QueryFloatAttribute("colorr", &colorr);
		plight->QueryFloatAttribute("colorg", &colorg);
		plight->QueryFloatAttribute("colorb", &colorb);
		DirectionalLight* dir = new DirectionalLight(glm::vec3(dirx, diry, dirz), glm::vec3(colorr, colorg, colorb));
		world->dirLights.push_back(dir);
	}
	*/
	return 0;// world;
}

World::~World() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		delete this->meshes[i];
	}
	for (unsigned int i = 0; i < meshes_nm.size(); i++) {
		delete this->meshes_nm[i];
	}
	for (unsigned int i = 0; i < meshes_nm.size(); i++) {
		delete this->meshes_inst[i];
	}
	for (unsigned int i = 0; i < meshes_free.size(); i++) {
		delete this->meshes_free[i];
	}
	for (unsigned int i = 0; i < terrains.size(); i++) {
		delete this->terrains[i];
	}
	for (auto &elem : this->models) {
		delete elem.second;
	}
	for (unsigned int i = 0; i < dirLights.size(); i++) {
		delete this->dirLights[i];
	}
	delete cam;
	delete basic;
	delete water;
}