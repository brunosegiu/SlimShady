#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 worldTransform;
uniform vec3 cameraPos;
uniform float phi;

const float PI = 3.1415926535897932384626433832795;
const float amplitude = 0.08;
const vec3 lightdir = normalize(vec3(-1.0,-1.0,0.0));
const vec3 watercolor = vec3(0.1,0.1,0.45);
const vec3 speccol = vec3(1.0,1.0,1.0);
const float reffactor = 0.5; 

out vec3 color;
out vec3 normal;

float generateHeight(){
	float wave1 = sin(phi + position.x-position.z)*amplitude;
	float wave2 = sin(phi + position.z)*amplitude;
	float wave3 = sin(phi + position.x)*amplitude;
	float wave4 = sin(phi + position.x*3)*amplitude;
	return wave1+wave2+wave3;
}

vec3 gradient(){ //F(x,y,x) = y - f(x,z)
	vec3 wave1 = vec3(cos(phi + position.x-position.z)*amplitude, 0.5,-cos(phi + position.x-position.z)*amplitude);
	vec3 wave2 = vec3(0.0, 0.5, cos(phi + position.z)*amplitude);
	vec3 wave3 = vec3(cos(phi + position.x)*amplitude, 0.5, 0.0);
	vec3 wave4 = vec3(cos(phi + position.x*3)*amplitude, 0.5,0.0);
	return normalize(wave1+wave2+wave3);
}

vec3 specular(vec3 cameraPos, vec3 normal){
	vec3 view = normalize(cameraPos - gl_Position.xyz);
	vec3 reflected = normalize((worldTransform*vec4(reflect(-lightdir, normal),1.0)).xyz);
	float factor = max(dot(reflected,view),0);
	return speccol*pow(factor,15)*reffactor;
}

void main(){
  gl_Position =  worldTransform*vec4(position.x,position.y + generateHeight(),position.z,1.0);
  vec3 normal = gradient();
  vec3 specular = specular(cameraPos, normal);
  float factor = max(dot(-lightdir,normal),0.0);
  color = (watercolor*factor)+specular;
}