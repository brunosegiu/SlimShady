/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

#include <gl/glew.h>
#include <vector>
#include <map>
#include <aiVector3D.h>
#include <aiMatrix4x4.h>

class Bone;
struct aiMesh;
struct aiNode;
/*
 TO DO / PROBLEMS:
 - Add some kind of position variable? Maybe m_transform can be used as this. Unsure, since it's the transformation from the mesh's node.
 - When copying the meshes get the same name. This does not work with the animation-code since it compares meshes by name. I guess the mesh could be automatically named from an game level editor or so.
*/

// A mesh loads a chunk of vertices and send it's to the GPU. It also keeps track of all it's bones and the bones absolute transformations (calculated by the animtion)
class Mesh_Anim
{
public:
	typedef std::vector<Bone*> BoneVector;
	typedef std::map<std::string, Bone*> BoneMap;
	typedef std::vector<aiMatrix4x4> aiMatrix4x4Vector;

	// Constructor will find which bones are necessary for this mesh and copy the bones. Will also go through all the vertices and put them into a list together with bone influences and weights. These vertices are sent to the GPU in a VBO.
	Mesh_Anim(const aiMesh* assimpMesh, const aiNode* meshNode, aiNode* rootNode, const aiMatrix4x4& transform);
	Mesh_Anim(const Mesh_Anim& rightHand);
	Mesh_Anim& operator=(const Mesh_Anim& rightHand);
	~Mesh_Anim();

	const aiMatrix4x4& getTransform() const { return m_transform; }
	const std::string& getName() { return m_name; }
	const BoneVector& getBones() const { return m_bones; }
	size_t getNumBones() const { return m_bones.size(); }
	size_t getBoneIndex(const std::string& name) const { if (m_boneNameToIndex.count(name) > 0) return m_boneNameToIndex.at(name); else return NULL; }
	Bone* getBoneByName(const std::string& name) const { if (m_bonesByName.count(name) > 0) return m_bonesByName.at(name); else return NULL; }
	// Set pointer to first transform in absolute transforms vector. We use m_bones.size() for the size of it
	void setAbsoluteTransforms(const GLfloat* transforms) { m_absoluteTransforms = transforms; }
	void setName(const std::string& name) { m_name = name; }

	// Calls updateTransforms for the root bone (and therefore all child bones)
	void updateTransforms();
	bool hasBone(const std::string& name) const;
	void draw(GLuint bonesLocation) const;

private:
	// These two are used by copy constructor, assignment operator and destructor
	void internalClear();
	Bone* copySkeletonUsingNecessityMap(const aiNode* node, Bone* parent, const std::map<std::string, bool>& boneNecessary);

	std::string			m_name;
	BoneVector			m_bones;
	std::map<std::string, size_t> m_boneNameToIndex;
	BoneMap				m_bonesByName;
	Bone*				m_rootBone;
	aiMatrix4x4			m_transform;
	// These vectors actually reside in the animation
	const GLfloat*		m_absoluteTransforms;
	GLuint				m_numIndices;
	GLuint				m_vertexBuffer;
	GLuint				m_indexBuffer;
	GLuint				vaoID;
};
