/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

#include <gl/glew.h>
#include <vector>
#include "AnimationController.h"
#include <aiVector3D.h>
#include <aiMatrix4x4.h>
#include <map>
#include "Mesh_Anim.h"
#include "Material.h"

struct aiScene;
struct aiNode;
// A model contains a animation controller and some meshes. It simply delegates the construction, updating and drawing of these objects.
class Model_Anim
{
public:
	Model_Anim(const aiScene* scene);
	Model_Anim(const Model_Anim& rightHand);
	Model_Anim& operator=(const Model_Anim& rightHand);
	~Model_Anim();
	void draw(GLuint bonesLocation) const;
	void update(float time);
private:
	void internalClear();
	void processNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform);

	AnimationController		m_animationController;
	std::vector<Mesh_Anim*>		m_meshes;
	Material* mat;
};

