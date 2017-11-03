#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 worldTransform;
uniform float phi;

const float PI = 3.1415926535897932384626433832795;
const float amplitude = 0.5;
//const vec3 watercolor = vec3(0.2,0.2,0.5);

out vec3 normal;
out vec2 passTextCoord;

float generateHeight(){
	float wave1 = sin(phi + position.z/16)*0.5;			//Deep Water
	float wave2 = sin(phi + position.x/3)*0.5;
	float wave3 = sin(phi + position.x/7)*0.5;
	float wave4 = sin(phi + position.x/16)*2;
	return (wave1+wave2+wave3+wave4)*amplitude;
}

vec3 gradient(){ //F(x,y,x) = y - f(x,z)
	vec3 wave1 = vec3(0.0, 0.3, cos(phi + position.z/16)/16*0.3);
	vec3 wave2 = vec3(cos(phi + position.x/3)/3*0.5, 0.3, 0.0);
	vec3 wave3 = vec3(cos(phi + position.x/7)/7*0.5, 0.3,0.0);
	vec3 wave4 = vec3(cos(phi + position.x/16)/16*2, 0.3, 0.0);
	return normalize(wave1+wave2+wave3+wave4);
}

void main(){
  gl_Position =  worldTransform*vec4(position.x,position.y + generateHeight(),position.z,1.0);
  normal = gradient();
  passTextCoord = textureCoord;
}