// Written by Karl Zylinski
// Portfolio and website: http://zylinski.se
// Use, modify and copy in any way you want

#version 330

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec4 vInfluences;
layout(location = 4) in vec4 vWeights;
layout(location = 5) in float vNumWeights;

uniform mat4 worldTransform;
uniform mat4 bones[250];	// All bones come in a huge list!

out vec3 normal;
out vec2 texCoord;

void main()
{
	texCoord = vTexCoord;
	normal = vNormal;

	// Used to mesure how much weight we've added together, should end up at 1.0
	float totalWeight = 0;

	// This is the position and normal which will be calculated using the influencing bones
	vec4 position = vec4(0,0,0,1);
	vec3 normal = vec3(0,0,0);

	// vNumWeights will be a number between 1 and 3
	for(int i = 0; i < int(vNumWeights); i++)
	{
		// A weight is a number beteen 0 and 1, the number fetched from the influence list is the index in the bones array
		position += vWeights[i] * vec4(bones[int(vInfluences[i])] * vPosition);
		normal += vWeights[i] * vec3(bones[int(vInfluences[i])] * vec4(vNormal,1));

		totalWeight += vWeights[i];
	}
	
	// totalWeight _should_ be 1, we adjust the position just in case it isn't
	position /= totalWeight;
	position.w = 1; // w will problaby have a weird value after all those additions
	normalize(normal);
	gl_Position =  worldTransform * position;
}