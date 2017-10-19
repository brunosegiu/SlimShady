#version 330 core

layout (location = 0) in vec3 position;

out vec3 color;

uniform mat4 worldTransform;
uniform float phi;

const float PI = 3.1415926535897932384626433832795;
const float amplitude = 0.01;

float generateHeight(){
	return sin(2*PI*position.x + mod(phi,2*PI) + position.x);
}

void main(){
  gl_Position =  worldTransform * vec4(position.x,generateHeight(),position.z,1.0);
  color = vec3(1.0,1.0,1.0);
}