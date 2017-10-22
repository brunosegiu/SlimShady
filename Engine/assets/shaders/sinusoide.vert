#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 worldTransform;
uniform float phi;

const float PI = 3.1415926535897932384626433832795;
const float amplitude = 0.01;

out vec3 vcolor;

float generateHeight(){
	return (sin(2*PI*position.x + mod(phi,2*PI) + position.x) + sin(2*PI*position.z + mod(phi,2*PI) + position.z)) * 0.5;
}

void main(){
  gl_Position =  worldTransform * vec4(position.x,generateHeight(),position.z,1.0);
  vcolor = vec3(0.0,0.0,1.0);
}