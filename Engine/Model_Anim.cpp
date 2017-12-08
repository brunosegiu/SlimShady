/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "Mesh_Anim.h"
#include "Model_Anim.h"
#include "Bone.h"
#include <aiScene.h>

using namespace std;

Model_Anim::Model_Anim(const aiScene* scene):
m_animationController(scene)
{
	processNode(scene->mRootNode, scene, aiMatrix4x4());
	m_animationController.precalculate(m_meshes);
}

Model_Anim::~Model_Anim()
{
	internalClear();
}

Model_Anim::Model_Anim(const Model_Anim& rightHand):
m_animationController(rightHand.m_animationController)
{
	m_animationController.reset();

	m_meshes.resize(rightHand.m_meshes.size());
	for(size_t meshIndex = 0; meshIndex < rightHand.m_meshes.size(); ++meshIndex)
		m_meshes[meshIndex] = new Mesh_Anim(*rightHand.m_meshes[meshIndex]);
}

Model_Anim& Model_Anim::operator=(const Model_Anim& rightHand)
{
	if(this != &rightHand)
	{
		internalClear();

		m_animationController = rightHand.m_animationController;
		m_animationController.reset();

		m_meshes.resize(rightHand.m_meshes.size());
		for(size_t meshIndex = 0; meshIndex < rightHand.m_meshes.size(); ++meshIndex)
			m_meshes[meshIndex] = new Mesh_Anim(*rightHand.m_meshes[meshIndex]);
	}

	return *this;
}

void Model_Anim::internalClear()
{
	for(size_t meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
		delete m_meshes[meshIndex];
}

void Model_Anim::processNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform)
{
	// It's important that we keep the nodes transformation even though we don't create a mesh
	transform *= node->mTransformation;

	if(node->mNumMeshes > 0)
	{
		for(size_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
		{
			Mesh_Anim* newMesh = new Mesh_Anim(scene->mMeshes[node->mMeshes[meshIndex]], node, const_cast<aiNode*>(scene->mRootNode), transform);
			m_meshes.push_back(newMesh);
		}
	}

	for(size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
		processNode(node->mChildren[childIndex], scene, transform);
}

void Model_Anim::draw(GLuint bonesLocation) const
{
	for(size_t meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
	{
		m_meshes[meshIndex]->draw(bonesLocation);
	}
}

void Model_Anim::update(float time)
{
	m_animationController.update(time, m_meshes);
}
