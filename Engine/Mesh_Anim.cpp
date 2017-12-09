/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "Mesh_Anim.h"
#include <aiMesh.h>
#include "Vertex.h"
#include "Bone.h"

using namespace std;

void initializeBoneNecessityMap(const aiNode* node, std::map<std::string, bool>& boneNecessary)
{
	boneNecessary[node->mName.data] = false;

	for(size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
	{
		initializeBoneNecessityMap(node->mChildren[childIndex], boneNecessary);
	}
}

const aiNode* recursivelyMarkBoneNecessary(const aiNode* node, const std::string& meshName, const std::string& meshParentName, std::map<std::string, bool>& boneNecessary)
{
	boneNecessary[node->mName.data] = true;

	if(node->mParent->mName.data == meshName || node->mParent->mName.data == meshParentName)
		return node;

	return recursivelyMarkBoneNecessary(node->mParent, meshName, meshParentName, boneNecessary);
}

Bone* Mesh_Anim::copySkeletonUsingNecessityMap(const aiNode* node, Bone* parent, const std::map<std::string, bool>& boneNecessary)
{
	Bone* rootBone = 0;

	if(boneNecessary.at(node->mName.data) == true)
	{
		Bone* newBone = new Bone(node, parent);
		m_bonesByName[newBone->getName()] = newBone;
		m_boneNameToIndex[newBone->getName()] = m_bones.size(); // We add the bone below, so .size is ok
		m_bones.push_back(newBone);

		if(parent == 0)
		{
			rootBone = newBone; // ugly hack :(
		}
		else
		{
			parent->addChild(newBone);
		}

		for(size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
		{
			copySkeletonUsingNecessityMap(node->mChildren[childIndex], newBone, boneNecessary);
		}
	}

	return rootBone;
}

Mesh_Anim::Mesh_Anim(const aiMesh* assimpMesh, const aiNode* meshNode, aiNode* rootNode, const aiMatrix4x4& transform):
m_name(meshNode->mName.data),
m_transform(transform)
{

	m_transform.Transpose();

	std::map<std::string, bool> boneNecessary;
	initializeBoneNecessityMap(rootNode, boneNecessary);
	const aiNode* rootBoneNode = 0;
	for(size_t boneIndex = 0; boneIndex < assimpMesh->mNumBones; ++boneIndex)
	{
		aiBone* bone = assimpMesh->mBones[boneIndex];
		aiNode* foundNode = rootNode->FindNode(bone->mName);
		if(foundNode != 0) // there is a bone and a node with this name
		{
			// all bones up to the mesh node or the mesh nodes parent will be marked as necessary
			rootBoneNode = recursivelyMarkBoneNecessary 
			(
				foundNode,
				m_name,
				rootNode->FindNode(m_name)->mParent->mName.data,
				boneNecessary
			);
		}
	}
	
	// Things might get quite fucked up if this isn't true...
	if(rootBoneNode)
	{
		// Recursivley copies the skeleton
		m_rootBone = copySkeletonUsingNecessityMap(rootBoneNode, 0, boneNecessary);
		// Set bind pose
		m_rootBone->updateTransforms();
	}
	
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	for(size_t faceIndex = 0; faceIndex < assimpMesh->mNumFaces; ++faceIndex)
	{
		auto face = assimpMesh->mFaces[faceIndex];
					
		for(size_t i = 0; i < face.mNumIndices; ++i)
		{
			Vertex v;
						
			GLuint index = face.mIndices[i];
			if(assimpMesh->mVertices[index] != 0)
			{
				v.m_position[0] = assimpMesh->mVertices[index].x;
				v.m_position[1] = assimpMesh->mVertices[index].y;
				v.m_position[2] = assimpMesh->mVertices[index].z;
			}
			else
			{
				v.m_position[0] = 0;
				v.m_position[1] = 0;
				v.m_position[2] = 0;
			}

			if(assimpMesh->mNormals && assimpMesh->mNormals[index] != 0)
			{
				v.m_normal[0] = assimpMesh->mNormals[index].x;
				v.m_normal[1] = assimpMesh->mNormals[index].y;
				v.m_normal[2] = assimpMesh->mNormals[index].z;
			}
			else
			{
				v.m_normal[0] = 0;
				v.m_normal[1] = 0;
				v.m_normal[2] = 0;
			}

			if(assimpMesh->mTextureCoords && assimpMesh->mTextureCoords[0] != 0)
			{
				v.m_texCoord[0] = assimpMesh->mTextureCoords[0][index].x;
				v.m_texCoord[1] = assimpMesh->mTextureCoords[0][index].y;
			}
			else
			{
				v.m_texCoord[0] = 0;
				v.m_texCoord[1] = 0;
			}
			
			size_t searchVertexIndex = 0;
			for(; searchVertexIndex != vertices.size(); ++searchVertexIndex)
			{
				if(v == vertices[searchVertexIndex])
				{
					break;
				}
			}

			if(searchVertexIndex == vertices.size())
			{
				for(size_t boneIndex = 0; boneIndex < assimpMesh->mNumBones; ++boneIndex)
				{
					auto bone = assimpMesh->mBones[boneIndex];

					size_t actualBoneIndex = 0;
					for(; actualBoneIndex < m_bones.size(); ++actualBoneIndex)
					{
						if(m_bones[actualBoneIndex]->getName() == bone->mName.data)
						{
							// Weird place to set offsetMatrix?
							m_bones[actualBoneIndex]->setOffsetMatrix(bone->mOffsetMatrix);
							break;
						}
					}

					if(actualBoneIndex == m_bones.size())
						continue;

					for(size_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
					{
						auto weight = bone->mWeights[weightIndex];

						if(weight.mVertexId == index)
						{
							int numWeights = v.m_numBoneWeights;
							if(numWeights < 4)
							{
								v.m_boneInfluences[numWeights] = actualBoneIndex;
								v.m_boneWeights[numWeights] = weight.mWeight;
								v.m_numBoneWeights++;
							}
						}
					}
				}

				vertices.push_back(v);
			}
			
			// Will be set to the found vertex or the size of the vertex-list (both are correct)
			indices.push_back(searchVertexIndex);
		}
	}

	m_numIndices = indices.size();

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoord));
	glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_boneInfluences));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_boneWeights));
	glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_numBoneWeights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Used by copy constructor and destructor
void Mesh_Anim::internalClear()
{
	/*
	We can't delete the buffers here since the data is shared between copied meshes.
	This needs some kind of buffer-mesh-factory-layer-thing to be 100% memory leak free.
	
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
	*/

	for(size_t boneIndex = 0; boneIndex < m_bones.size(); ++boneIndex)
		delete m_bones[boneIndex];
}

Mesh_Anim::Mesh_Anim(const Mesh_Anim& rightHand):
m_name(rightHand.m_name),
m_boneNameToIndex(rightHand.m_boneNameToIndex),
m_absoluteTransforms(0),
m_numIndices(rightHand.m_numIndices),
m_vertexBuffer(rightHand.m_vertexBuffer),
m_indexBuffer(rightHand.m_indexBuffer),
m_transform(rightHand.m_transform)
{
	m_bones.resize(rightHand.m_bones.size());
	for(size_t boneIndex = 0; boneIndex < rightHand.m_bones.size(); ++boneIndex)
	{
		Bone* newBone = new Bone(*rightHand.m_bones[boneIndex]);
		m_bones[boneIndex] = newBone;
		m_bonesByName[newBone->getName()] = newBone;
	}

	// Need to find our copy of the rootbone, can be done by name
	if(rightHand.m_rootBone)
	{
		m_rootBone = m_bonesByName.at(rightHand.m_rootBone->getName());
		m_rootBone->updateChildren(m_bonesByName);
	}
	else
		m_rootBone = 0;
}

Mesh_Anim::~Mesh_Anim()
{
	//internalClear();	
}

Mesh_Anim& Mesh_Anim::operator=(const Mesh_Anim& rightHand)
{
	if(this != &rightHand)
	{
		internalClear();
		m_name = rightHand.m_name;
		m_boneNameToIndex = rightHand.m_boneNameToIndex;
		m_absoluteTransforms = 0; // will get new ptr from animationcontroller
		m_numIndices = rightHand.m_numIndices;
		// The buffers are just pointers to opengl buffers
		m_vertexBuffer = rightHand.m_vertexBuffer;
		m_indexBuffer = rightHand.m_indexBuffer;
		m_transform = rightHand.m_transform;

		// Dynamic memory
		m_bones.resize(rightHand.m_bones.size());
		for(size_t boneIndex = 0; boneIndex < rightHand.m_bones.size(); ++boneIndex)
		{
			Bone* newBone = new Bone(*rightHand.m_bones[boneIndex]);
			m_bones[boneIndex] = newBone;
			m_bonesByName[newBone->getName()] = newBone;
		}

		// Need to find our copy of the rootbone, can be done by name
		if(rightHand.m_rootBone)
		{
			m_rootBone = m_bonesByName.at(rightHand.m_rootBone->getName());
			m_rootBone->updateChildren(m_bonesByName);
		}
		else
			m_rootBone = 0;
	}

	return *this;
}

void Mesh_Anim::updateTransforms()
{
	m_rootBone->updateTransforms();
}

bool Mesh_Anim::hasBone(const std::string& name) const
{
	return(m_bonesByName.find(name) != m_bonesByName.end());
}

void Mesh_Anim::draw(GLuint bonesLocation) const
{
	glUniformMatrix4fv(bonesLocation, m_bones.size(), GL_FALSE, m_absoluteTransforms);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}