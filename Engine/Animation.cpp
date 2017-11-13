#include "Animation.h"

#include "tinyxml2.h"

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

vector<unsigned int> toFloatBufferINT(char delimiter, string &input) {
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
	if (temp != "")
		final.push_back(stof(temp));
	return final;
}

Animation::Animation(string path) {
	using namespace tinyxml2;
	XMLDocument doc;
	doc.LoadFile(path.c_str());

	// Load geometry

	vector<float> tempVertexBuffer;
	vector<float> tempNormalBuffer;
	vector<float> tempTextureBuffer;
	vector<unsigned int> indexBuffer;

	XMLNode* proot = doc.FirstChildElement("COLLADA");
	XMLNode* pgeom = proot->FirstChildElement("library_geometries");
	XMLNode* meshData = pgeom->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("source");

	XMLElement* vertices = meshData->FirstChildElement("float_array");
	string values = vertices->GetText();
	tempVertexBuffer = toFloatBuffer(' ', values);

	meshData = meshData->NextSibling();

	XMLElement* normals = meshData->FirstChildElement("float_array");
	values = normals->GetText();
	tempNormalBuffer = toFloatBuffer(' ', values);

	meshData = meshData->NextSibling();
	XMLElement* text = meshData->FirstChildElement("float_array");
	values = text->GetText();
	tempTextureBuffer = toFloatBuffer(' ', values);

	OBJIndexer indexer = OBJIndexer();
	XMLNode* indexData = pgeom->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("polylist");
	XMLElement* indices = indexData->FirstChildElement("p");
	values = indices->GetText();
	vector<unsigned int> tempIndexBuffer = toFloatBufferINT(' ', values);
	for (unsigned int i = 0; i < tempIndexBuffer.size(); i+=3) {
		unsigned int index = indexer.getIndex(tempVertexBuffer[3 * tempIndexBuffer[i]], tempVertexBuffer[3 * tempIndexBuffer[i] + 1], tempVertexBuffer[3 * tempIndexBuffer[i] + 2],
											  tempNormalBuffer[3 * tempIndexBuffer[i+1]], tempNormalBuffer[3 * tempIndexBuffer[i+1] + 1], tempNormalBuffer[3 * tempIndexBuffer[i+1] + 2],
											  tempTextureBuffer[2 * tempIndexBuffer[i+2]], tempTextureBuffer[2 * tempIndexBuffer[i+2] + 1]);
		indexBuffer.push_back(index);
	}
	
	// Load skin

	vector<float> weights;
	XMLNode* pcontroller = proot->FirstChildElement("library_controllers");
	XMLNode* pskin = pcontroller->FirstChildElement("controller")->FirstChildElement("skin");
	XMLElement* pweight = pskin->FirstChildElement("");

}