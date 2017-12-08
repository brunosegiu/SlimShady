#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 worldTransform;

void main(){
	gl_Position = worldTransform*vec4(pos,1.0);
	gl_PointSize = 10.0;
}