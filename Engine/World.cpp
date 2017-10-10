#include "World.h"

World::World() {

}

void World::draw() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		this->meshes[i].draw();
	}
}