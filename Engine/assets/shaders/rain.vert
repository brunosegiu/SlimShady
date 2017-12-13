#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 massVec;

uniform mat4 worldTransform;
out float mass;

void main(){
	gl_Position = worldTransform*vec4(pos,1.0);
	mass = massVec.x;
}