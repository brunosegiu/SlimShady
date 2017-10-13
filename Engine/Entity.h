#pragma once

#include<glm/mat4x4.hpp>

#include "Mesh.h"

/*Si bien parece al pedo, por ahora, una entidad puede servir como wrapper de animaciones y meshes (y alguna otra cosa que no se me ocurre)*/

class Entity {
	public:
		Entity(Mesh* mesh);
		//Entity(Animation* anim);
		void applyTransform(glm::mat4 &transform);
		void draw(GLuint shaderID);
		~Entity();

		glm::mat4 modelMatrix;
		Mesh* mesh;
};