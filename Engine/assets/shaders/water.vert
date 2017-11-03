#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 worldTransform;
uniform vec3 cameraPos;
uniform float phi;
uniform vec3 lightdir;
uniform vec3 lightcolor;

const float PI = 3.1415926535897932384626433832795;
const float amplitude = 0.5;
//const vec3 lightdir = normalize(vec3(-1.0,-1.0,0.0));
const vec3 watercolor = vec3(0.2,0.2,0.5);
const vec3 speccol = vec3(1.0,1.0,1.0);
const float reffactor = 0.5; 

out vec3 color;
out vec3 normal;

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
  float factor = max(dot(-lightdir,normal),0.3);
  color = (watercolor*lightcolor*factor)+specular;
}