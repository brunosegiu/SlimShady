#pragma once

#include<glm/mat4x4.hpp>

#include "Mesh.h"

/*Si bien parece al pedo, por ahora, una entidad puede servir como wrapper de animaciones y meshes (y alguna otra cosa que no se me ocurre)*/

class Entity {
	public:
		Entity(Mesh* mesh);
		//Entity(Animation* anim);
		void rotate(float angle, glm::vec3 &dir);
		void translate(glm::vec3 &trs);
		void scale(glm::vec3 &scale);
		void scaleInOrigin(glm::vec3 &scale);
		void draw(GLuint shaderID);
		~Entity();

		glm::mat4 modelMatrix;
		glm::mat4 normalRotations;
		Mesh* mesh;
};