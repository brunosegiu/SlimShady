#include "Animation.h"

#include "tinyxml2.h"
#include <glm/glm.hpp>

#include "OBJIndexer.h"

vector<float> toFloatBuffer(char delimiter, string &input) {
	string temp = "" ;
	vector<float> final;
	for (unsigned int i = 0; i < input.size(); i++) {
		if (input[i] == delimiter && temp.size() > 0) {
			final.push_back(stof(temp));
			temp = "";
		}
		else {
			temp += input[i];
		}
	}
	if (temp != "")
		final.push_back(stof(temp));
	return final;
}

vector<string> split(char delimiter, string &input) {
	string temp = "";
	vector<string> final;
	for (unsigned int i = 0; i < input.size(); i++) {
		if (input[i] == delimiter && temp.size() > 0) {
			final.push_back(temp);
			temp = "";
		}
		else {
			temp += input[i];
		}
	}
	if (temp != "")
		final.push_back(temp);
	return final;
}

vector<unsigned int> toFloatBufferUINT(char delimiter, string &input) {
	string temp = "";
	vector<unsigned int> final;
	for (unsigned int i = 0; i < input.size(); i++) {
		if (input[i] == delimiter && temp.size() > 0) {
			final.push_back(stof(temp));
			temp = "";
		}
		else {
			temp += input[i];
		}
	}
	if (temp != "")
		final.push_back(stof(temp));
	return final;
}

vector<unsigned int> parseIndices(char delimiter, string &input) {
	string temp = "";
	int count = 0;
	vector<unsigned int> final;
	for (unsigned int i = 0; i < input.size(); i++) {
		if (input[i] == delimiter && temp.size() > 0) {
			count++;
			if (count % 4 != 0)
				final.push_back(stoi(temp));
			temp = "";
		}
		else {
			temp += input[i];
		}
	}
	if (temp != "" && final.size() % 3 != 0)
		final.push_back(stof(temp));
	return final;
}

int findPos(string name, vector<string> &jointNames) {
	for (unsigned int i = 0; i < jointNames.size(); i++) {
		if (jointNames[i] == name) {
			return i;
		}
	}
	throw exception("NOOOOOOOOOOOO");
}

Joint* loadJoints(tinyxml2::XMLElement* root, vector<string> &jointNames) {
	using namespace tinyxml2;
	string name = root->Attribute("id");
	int pos = findPos(name, jointNames);
	Joint* joint = new Joint(name, pos);
	string matrix = root->FirstChildElement("matrix")->GetText();
	vector<float> mat = toFloatBuffer(' ', matrix);
	glm::mat4 transfmat = glm::mat4(mat[0], mat[4], mat[8], mat[12], mat[1], mat[5], mat[9], mat[13], mat[2], mat[6], mat[10], mat[14], mat[3], mat[7], mat[11], mat[15]);
	joint->transform = transfmat;
	for (XMLElement* child = root->FirstChildElement("node"); child != NULL; child = child->NextSiblingElement("node")){
		string id = child->Attribute("id");
		joint->children.push_back(loadJoints(child, jointNames));
	}
	return joint;
}

Animation::Animation(string path) {
	elapsed = 0;

	using namespace tinyxml2;
	XMLDocument doc;
	doc.LoadFile(path.c_str());

	XMLNode* pgeom = doc.FirstChildElement("geometry");
	XMLNode* panim = doc.FirstChildElement("animation");
	XMLNode* pskeleton = doc.FirstChildElement("skeleton");

	//Load geometry
	vector<float> vertexBuffer;
	vector<float> normalBuffer;
	vector<float> textBuffer;
	vector<unsigned int> indexBuffer;
	vector<float> weightBuffer;
	vector<unsigned int> jointCount;
	vector<unsigned int> skinBuffer;

	string vertices = pgeom->FirstChildElement("vertices")->GetText();
	vertexBuffer = toFloatBuffer(' ', vertices);

	string normals = pgeom->FirstChildElement("normals")->GetText();
	normalBuffer = toFloatBuffer(' ', normals);

	string uvmap = pgeom->FirstChildElement("uvmap")->GetText();
	textBuffer = toFloatBuffer(' ', uvmap);

	string indices = pgeom->FirstChildElement("indices")->GetText();
	indexBuffer = parseIndices(' ', indices);

	string weights = pgeom->FirstChildElement("weights")->GetText();
	weightBuffer = toFloatBuffer(' ', weights);

	string sjointCounts = pgeom->FirstChildElement("jointCount")->GetText();
	jointCount = toFloatBufferUINT(' ', sjointCounts);

	string sskin = pgeom->FirstChildElement("skin")->GetText();
	skinBuffer = toFloatBufferUINT(' ', sskin);

	vector<float> finalWeights;
	vector<unsigned int> finalJointIDs;
	vector<float> finalNormalBuffer, finalVertexBuffer, finalUVBuffer;
	vector<unsigned int> finalIndices;
	unsigned int k = 0;
	for (unsigned int i = 0; i < indexBuffer.size(); i += 3) {
		finalIndices.push_back(k);
		k++;
		finalVertexBuffer.push_back(vertexBuffer[indexBuffer[i]]);
		finalVertexBuffer.push_back(vertexBuffer[indexBuffer[i]+1]);
		finalVertexBuffer.push_back(vertexBuffer[indexBuffer[i]+2]);
		finalNormalBuffer.push_back(normalBuffer[indexBuffer[i+1]]);
		finalNormalBuffer.push_back(normalBuffer[indexBuffer[i+1]+1]);
		finalNormalBuffer.push_back(normalBuffer[indexBuffer[i+1]+2]);
		finalUVBuffer.push_back(textBuffer[indexBuffer[i+2]]);
		finalUVBuffer.push_back(textBuffer[indexBuffer[i+2] + 1]);
	}
	//Fix JC
	unsigned int sbit = 0;
	for (unsigned int i = 0; i < jointCount.size(); i++) {
		vector<float> ws;
		vector<unsigned int> joints;
		ws.push_back(0);
		ws.push_back(0);
		ws.push_back(0);
		joints.push_back(0);
		joints.push_back(0);
		joints.push_back(0);
		for (unsigned int j = 0; j < jointCount[i] && j < 3; j++) {
			joints[j] = skinBuffer[sbit];
			ws[j] = weightBuffer[skinBuffer[sbit+1]];
			sbit += 2;
		}
		if (int(jointCount[i]) - 3 > 0) {
			sbit += 2 * (jointCount[i] - 3);
		}
		glm::vec3 normalized = glm::vec3(ws[0], ws[1], ws[2]);
		normalized = glm::normalize(normalized);
		finalJointIDs.push_back(joints[0]);
		finalJointIDs.push_back(joints[1]);
		finalJointIDs.push_back(joints[2]);
		finalWeights.push_back(normalized.x);
		finalWeights.push_back(normalized.y);
		finalWeights.push_back(normalized.z);
	}

	// Skeleton
	vector<string> jointNames = split(' ', string(pskeleton->FirstChildElement("jointNames")->GetText()));

	this->root = loadJoints(pskeleton->FirstChildElement("node"), jointNames);
	this->root->calcBindMatrix(glm::mat4(1.0f));
	//Animation

	//GPU
	GLuint verticesID, normalsID, textID, weightsID, jointsID;

	//Copy vertices to GPU
	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, verticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * finalVertexBuffer.size(), &finalVertexBuffer[0], GL_STATIC_DRAW);

	//Copy normals to GPU
	glGenBuffers(1, &normalsID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * finalNormalBuffer.size(), &finalNormalBuffer[0], GL_STATIC_DRAW);

	//Copy UV to GPU
	glGenBuffers(1, &textID);
	glBindBuffer(GL_ARRAY_BUFFER, textID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * finalUVBuffer.size(), &finalUVBuffer[0], GL_STATIC_DRAW);

	//Copy WS to GPU
	glGenBuffers(1, &weightsID);
	glBindBuffer(GL_ARRAY_BUFFER, weightsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * finalWeights.size(), &finalWeights[0], GL_STATIC_DRAW);

	//Copy UV to GPU
	glGenBuffers(1, &jointsID);
	glBindBuffer(GL_ARRAY_BUFFER, jointsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * finalJointIDs.size(), &finalJointIDs[0], GL_STATIC_DRAW);

	//Set up the VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	//Bind Vertices
	glBindBuffer(GL_ARRAY_BUFFER, verticesID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Bind Normals
	glBindBuffer(GL_ARRAY_BUFFER, normalsID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Bind textures
	glBindBuffer(GL_ARRAY_BUFFER, textID);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	/*//Bind Normals
	glBindBuffer(GL_ARRAY_BUFFER, weightsID);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Bind Normals
	glBindBuffer(GL_ARRAY_BUFFER, jointsID);
	glVertexAttribPointer(4, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);*/
	//Indices
	glGenBuffers(1, &this->indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * finalIndices.size(), &finalIndices[0], GL_STATIC_DRAW);
	faces = finalIndices.size();
	jointNumber	= jointNames.size();

}

void Animation::draw(GLuint shaderID){
	vector<glm::mat4> matrices = vector<glm::mat4>(jointNumber);
	root->addMatrices(matrices);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "jointTransforms"), 1, GL_FALSE, &matrices[0][0][0]);
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	/*glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesID);
	glDrawElements(GL_TRIANGLES, this->faces, GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	/*glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);*/
}