#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in vec3 weights;
layout(location = 4) in ivec3 jointID;

uniform mat4 worldTransform;
uniform mat4 jointTransforms[50];

void main(){
	gl_Position = worldTransform /** jointTransforms[jointID.x] * jointTransforms[jointID.y] * jointTransforms[jointID.z]*/ * vec4(position,1);
}