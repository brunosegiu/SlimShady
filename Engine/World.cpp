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

	this->lastDraw = clock();

	this->sun = new Sun(lastDraw);
	this->sky = new Skybox(2000);

	this->filters.push_back(pair<bool, Filter*>(true, new Filter("assets/shaders/fxaa.frag", cam->width, cam->height)));
	this->filters.push_back(pair<bool, Filter*>(true, new Filter("assets/shaders/colorcorrection.frag", cam->width, cam->height)));
	this->filters.push_back(pair<bool, Filter*>(true, new Filter("assets/shaders/depthoffield.frag", cam->width, cam->height)));
	
	gamma = 1.0f;
	contrast = 1.0f;
	brightness = 0.0f;
	fogFactor = 1.0f;
	vignette = 0.2f;

	anim = new Animation("assets/models/cowboy.anim");
	animationShader = new ShaderProgram("assets/shaders/anim.vert", "assets/shaders/anim.frag");
}

void World::draw() {
	glEnable(GL_CULL_FACE);
	// Update drawing timer
	float elapsed = (clock() - this->lastDraw) / double(CLOCKS_PER_SEC);
	this->lastDraw = clock();
	float daylight = clock() / (double(CLOCKS_PER_SEC)*5);

	//Update Sun
	this->sun->updateLight(elapsed/5);

	// Update Camera view
	this->cam->update();
	Filter::fbo->bind();
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
	water->modelMatrix = glm::translate(water->acumulatedTranslate) * glm::scale(water->acumulatedScale) * glm::rotate(water->acumulatedRotate.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(water->acumulatedRotate.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(water->acumulatedRotate.z, glm::vec3(0.0f, 0.0f, 1.0f));;
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

	/*glDisable(GL_CULL_FACE);
	animationShader->bind();
	GLuint worldTransformID = glGetUniformLocation(animationShader->getId(), "worldTransform");
	glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	anim->draw(animationShader->getId());*/

	//Render Skybox
	sky->mvp = this->cam->modelViewProjectionMatrix;
	sky->lightColor = sun->light->color;
	sky->intensity = sun->intensity;
	sky->moonColor = sun->moon->color;
	sky->mIntensity = sun->mIntensity;
	//sky->lastDraw = daylight;
	sky->draw(0);

	// Bind del buffer de pantalla
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Filter::fbo->textid);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Filter::fbo->textDB);
	glDisable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < filters.size(); i++) {
		Filter* fil = filters[i].second;
		fil->bind();
		GLuint onID = glGetUniformLocation(fil->shader->getId(), "on");
		glUniform1i(onID, filters[i].first);
		GLuint texID = glGetUniformLocation(fil->shader->getId(), "sampler");
		glUniform1i(texID, 0);
		GLuint texIDDB = glGetUniformLocation(fil->shader->getId(), "samplerDB");
		glUniform1i(texIDDB, 1);
		GLuint invSizeID = glGetUniformLocation(fil->shader->getId(), "invScreenSize");
		glUniform2f(invSizeID, 1.0f / float(cam->width), 1.0f / float(cam->height));
		GLuint gammaID = glGetUniformLocation(fil->shader->getId(), "gamma");
		glUniform1f(gammaID, gamma);
		GLuint brightnessID = glGetUniformLocation(fil->shader->getId(), "brightness");
		glUniform1f(brightnessID, brightness);
		GLuint contrastID = glGetUniformLocation(fil->shader->getId(), "contrast");
		glUniform1f(contrastID, contrast);
		GLuint fogFactorID = glGetUniformLocation(fil->shader->getId(), "fogFactor");
		glUniform1f(fogFactorID, fogFactor);
		GLuint vignetteID = glGetUniformLocation(fil->shader->getId(), "vignette");
		glUniform1f(vignetteID, vignette);
		if (i == filters.size()-1) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		Filter::quad->draw();
	}
	glEnable(GL_DEPTH_TEST);
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
	using namespace tinyxml2;
	XMLDocument xmlDoc;

	XMLNode * pRoot = xmlDoc.NewElement("world");
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

	XMLElement * pmodels = xmlDoc.NewElement("models");
	for (auto const &entry : models) {
		bool insert = true;
		Mesh* mesh = dynamic_cast<Mesh*>(entry.second);
		NormalMappedMesh* nmmesh = dynamic_cast<NormalMappedMesh*>(entry.second);
		Terrain* terrmesh = dynamic_cast<Terrain*>(entry.second);
		Water* water = dynamic_cast<Water*>(entry.second);
		XMLElement * pmodel = xmlDoc.NewElement("item");
		if (mesh) {
			pmodel->SetAttribute("type", "model");
			pmodel->SetAttribute("name", mesh->name.c_str());
			pmodel->SetAttribute("path", mesh->path.c_str());
		}
		else if (nmmesh) {
			pmodel->SetAttribute("type", "nmmodel");
			pmodel->SetAttribute("name", nmmesh->name.c_str());
			pmodel->SetAttribute("path", nmmesh->path.c_str());
		}
		else if (terrmesh) {
			pmodel->SetAttribute("type", "terrain");
			pmodel->SetAttribute("name", terrmesh->name.c_str());
			pmodel->SetAttribute("path", terrmesh->path.c_str());
			pmodel->SetAttribute("maxHeight", terrmesh->maxHeight);
			pmodel->SetAttribute("tilesX", terrmesh->tilesX);
			pmodel->SetAttribute("tilesY", terrmesh->tilesY);
		}
		else if (water){
			insert = false;
		}
		if (insert) pmodels->InsertEndChild(pmodel);
	}
	pRoot->InsertEndChild(pmodels);

	XMLElement * pmentities = xmlDoc.NewElement("mesh-entities");
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		XMLElement * pentity = xmlDoc.NewElement("item");
		pentity->SetAttribute("model", meshes[i]->model->name.c_str());
		pentity->SetAttribute("trsx", meshes[i]->acumulatedTranslate.x);
		pentity->SetAttribute("trsy", meshes[i]->acumulatedTranslate.y);
		pentity->SetAttribute("trsz", meshes[i]->acumulatedTranslate.z);
		pentity->SetAttribute("rotx", meshes[i]->acumulatedRotate.x);
		pentity->SetAttribute("roty", meshes[i]->acumulatedRotate.y);
		pentity->SetAttribute("rotz", meshes[i]->acumulatedRotate.z);
		pentity->SetAttribute("scalex", meshes[i]->acumulatedScale.x);
		pentity->SetAttribute("scaley", meshes[i]->acumulatedScale.y);
		pentity->SetAttribute("scalez", meshes[i]->acumulatedScale.z);
		pmentities->InsertEndChild(pentity);
	}
	pRoot->InsertEndChild(pmentities);

	XMLElement * pnmmentities = xmlDoc.NewElement("nmmesh-entities");
	for (unsigned int i = 0; i < this->meshes_nm.size(); i++) {
		XMLElement * pentity = xmlDoc.NewElement("item");
		pentity->SetAttribute("model", meshes_nm[i]->model->name.c_str());
		pentity->SetAttribute("trsx", meshes_nm[i]->acumulatedTranslate.x);
		pentity->SetAttribute("trsy", meshes_nm[i]->acumulatedTranslate.y);
		pentity->SetAttribute("trsz", meshes_nm[i]->acumulatedTranslate.z);
		pentity->SetAttribute("rotx", meshes_nm[i]->acumulatedRotate.x);
		pentity->SetAttribute("roty", meshes_nm[i]->acumulatedRotate.y);
		pentity->SetAttribute("rotz", meshes_nm[i]->acumulatedRotate.z);
		pentity->SetAttribute("scalex", meshes_nm[i]->acumulatedScale.x);
		pentity->SetAttribute("scaley", meshes_nm[i]->acumulatedScale.y);
		pentity->SetAttribute("scalez", meshes_nm[i]->acumulatedScale.z);
		pnmmentities->InsertEndChild(pentity);
	}
	pRoot->InsertEndChild(pnmmentities);

	XMLElement * pterrentities = xmlDoc.NewElement("terrain-entities");
	for (unsigned int i = 0; i < this->terrains.size(); i++) {
		XMLElement * pentity = xmlDoc.NewElement("item");
		pentity->SetAttribute("model", terrains[i]->model->name.c_str());
		pentity->SetAttribute("trsx", terrains[i]->acumulatedTranslate.x);
		pentity->SetAttribute("trsy", terrains[i]->acumulatedTranslate.y);
		pentity->SetAttribute("trsz", terrains[i]->acumulatedTranslate.z);
		pentity->SetAttribute("rotx", terrains[i]->acumulatedRotate.x);
		pentity->SetAttribute("roty", terrains[i]->acumulatedRotate.y);
		pentity->SetAttribute("rotz", terrains[i]->acumulatedRotate.z);
		pentity->SetAttribute("scalex", terrains[i]->acumulatedScale.x);
		pentity->SetAttribute("scaley", terrains[i]->acumulatedScale.y);
		pentity->SetAttribute("scalez", terrains[i]->acumulatedScale.z);
		pterrentities->InsertEndChild(pentity);
	}
	pRoot->InsertEndChild(pterrentities);

	XMLElement* pwater = xmlDoc.NewElement("water");
	pwater->SetAttribute("model", "Water");
	pwater->SetAttribute("trsx", this->water->acumulatedTranslate.x);
	pwater->SetAttribute("trsy", this->water->acumulatedTranslate.y);
	pwater->SetAttribute("trsz", this->water->acumulatedTranslate.z);
	pwater->SetAttribute("rotx", this->water->acumulatedRotate.x);
	pwater->SetAttribute("roty", this->water->acumulatedRotate.y);
	pwater->SetAttribute("rotz", this->water->acumulatedRotate.z);
	pwater->SetAttribute("scalex", this->water->acumulatedScale.x);
	pwater->SetAttribute("scaley", this->water->acumulatedScale.y);
	pwater->SetAttribute("scalez", this->water->acumulatedScale.z);
	pRoot->InsertEndChild(pwater);

	xmlDoc.SaveFile(path.c_str());
}

World* World::load(string path, SDL_Window* win, float width, float height) {
	using namespace tinyxml2;
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
	World* world = new World(cam);

	XMLElement * pmodels = pRoot->FirstChildElement("models");
	for (XMLElement* pmodel = pmodels->FirstChildElement("item"); pmodel != NULL; pmodel = pmodel->NextSiblingElement("item")) {
		string path = std::string(pmodel->Attribute("path"));
		string type = std::string(pmodel->Attribute("type"));
		string name = std::string(pmodel->Attribute("name"));
		if (type == "model") {
			Mesh* m = new Mesh(path);
			m->name = name;
			world->addModel(m);
		}else if (type == "nmmodel") {
			NormalMappedMesh* m = new NormalMappedMesh(path);
			m->name = name;
			world->addModel(m);
		}
		else if (type == "terrain") {
			float height;
			int tilesX, tilesY;
			pmodel->QueryFloatAttribute("maxHeight", &height);
			pmodel->QueryIntAttribute("tilesX", &tilesX);
			pmodel->QueryIntAttribute("tilesY", &tilesY);
			Terrain* terr = new Terrain(path, height, tilesX, tilesY);
			world->addModel(terr);
		}
	}

	XMLElement * pmentities = pRoot->FirstChildElement("mesh-entities");
	for (XMLElement* pentity = pmentities->FirstChildElement("item"); pentity != NULL; pentity = pentity->NextSiblingElement("item")) {
		string model = std::string(pentity->Attribute("model"));
		float rotx, roty, rotz, trasx, trasy, trasz, scalex, scaley, scalez;
		pentity->QueryFloatAttribute("trsx", &trasx);
		pentity->QueryFloatAttribute("trsy", &trasy);
		pentity->QueryFloatAttribute("trsz", &trasz);
		pentity->QueryFloatAttribute("rotx", &rotx);
		pentity->QueryFloatAttribute("roty", &roty);
		pentity->QueryFloatAttribute("rotz", &rotz);
		pentity->QueryFloatAttribute("scalex", &scalex);
		pentity->QueryFloatAttribute("scaley", &scaley);
		pentity->QueryFloatAttribute("scalez", &scalez);
		glm::vec3 tras = glm::vec3(trasx, trasy, trasz);
		glm::vec3 rot = glm::vec3(rotx, roty, rotz);
		glm::vec3 scale = glm::vec3(scalex , scaley, scalez);
		
		Entity* ent = new Entity(world->models[model], world);
		ent->acumulatedRotate = rot;
		ent->acumulatedScale = scale;
		ent->acumulatedTranslate = tras;

		world->meshes.push_back(ent);
	}

	XMLElement * pnmmentities = pRoot->FirstChildElement("nmmesh-entities");
	for (XMLElement* pentity = pnmmentities->FirstChildElement("item"); pentity != NULL; pentity = pentity->NextSiblingElement("item")) {
		string model = std::string(pentity->Attribute("model"));
		float rotx, roty, rotz, trasx, trasy, trasz, scalex, scaley, scalez;
		pentity->QueryFloatAttribute("trsx", &trasx);
		pentity->QueryFloatAttribute("trsy", &trasy);
		pentity->QueryFloatAttribute("trsz", &trasz);
		pentity->QueryFloatAttribute("rotx", &rotx);
		pentity->QueryFloatAttribute("roty", &roty);
		pentity->QueryFloatAttribute("rotz", &rotz);
		pentity->QueryFloatAttribute("scalex", &scalex);
		pentity->QueryFloatAttribute("scaley", &scaley);
		pentity->QueryFloatAttribute("scalez", &scalez);
		glm::vec3 tras = glm::vec3(trasx, trasy, trasz);
		glm::vec3 rot = glm::vec3(rotx, roty, rotz);
		glm::vec3 scale = glm::vec3(scalex, scaley, scalez);

		Entity* ent = new Entity(world->models[model], world);
		ent->acumulatedRotate = rot;
		ent->acumulatedScale = scale;
		ent->acumulatedTranslate = tras;

		world->meshes_nm.push_back(ent);
	}

	XMLElement * pterrentities = pRoot->FirstChildElement("terrain-entities");
	for (XMLElement* pentity = pterrentities->FirstChildElement("item"); pentity != NULL; pentity = pentity->NextSiblingElement("item")) {
		string model = std::string(pentity->Attribute("model"));
		float rotx, roty, rotz, trasx, trasy, trasz, scalex, scaley, scalez;
		pentity->QueryFloatAttribute("trsx", &trasx);
		pentity->QueryFloatAttribute("trsy", &trasy);
		pentity->QueryFloatAttribute("trsz", &trasz);
		pentity->QueryFloatAttribute("rotx", &rotx);
		pentity->QueryFloatAttribute("roty", &roty);
		pentity->QueryFloatAttribute("rotz", &rotz);
		pentity->QueryFloatAttribute("scalex", &scalex);
		pentity->QueryFloatAttribute("scaley", &scaley);
		pentity->QueryFloatAttribute("scalez", &scalez);
		glm::vec3 tras = glm::vec3(trasx, trasy, trasz);
		glm::vec3 rot = glm::vec3(rotx, roty, rotz);
		glm::vec3 scale = glm::vec3(scalex, scaley, scalez);

		Entity* ent = new Entity(world->models[model], world);
		ent->acumulatedRotate = rot;
		ent->acumulatedScale = scale;
		ent->acumulatedTranslate = tras;

		world->terrains.push_back(ent);
	}

	XMLElement * pwater = pRoot->FirstChildElement("water");
	float rotx, roty, rotz, trasx, trasy, trasz, scalex, scaley, scalez;
	pwater->QueryFloatAttribute("trsx", &trasx);
	pwater->QueryFloatAttribute("trsy", &trasy);
	pwater->QueryFloatAttribute("trsz", &trasz);
	pwater->QueryFloatAttribute("rotx", &rotx);
	pwater->QueryFloatAttribute("roty", &roty);
	pwater->QueryFloatAttribute("rotz", &rotz);
	pwater->QueryFloatAttribute("scalex", &scalex);
	pwater->QueryFloatAttribute("scaley", &scaley);
	pwater->QueryFloatAttribute("scalez", &scalez);
	glm::vec3 tras = glm::vec3(trasx, trasy, trasz);
	glm::vec3 rot = glm::vec3(rotx, roty, rotz);
	glm::vec3 scale = glm::vec3(scalex, scaley, scalez);
	world->addModel(new Water(200, 200));
	world->water = new Entity(world->models["Water"], world);
	world->water->acumulatedRotate = rot;
	world->water->acumulatedScale = scale;
	world->water->acumulatedTranslate = tras;
	
	return world;
}

World::~World() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		delete this->meshes[i];
	}
	for (unsigned int i = 0; i < meshes_nm.size(); i++) {
		delete this->meshes_nm[i];
	}
	for (unsigned int i = 0; i < meshes_nm.size(); i++) {
//		delete this->meshes_inst[i];
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