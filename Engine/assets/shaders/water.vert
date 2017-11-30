#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 worldTransform;
uniform float phi;

const float PI = 3.1415926535897932384626433832795;
const float amplitude = 0.1;
//const vec3 watercolor = vec3(0.2,0.2,0.5);

out vec3 normal;
out vec2 passTextCoord;

float generateHeight(float t, vec2 dir, float longi, float amp, float n){
	return pow((sin(phi*t + (position.x*dir.x + position.z*dir.y)*longi)+1)/2,n)*amp;
}

vec3 gradient(float t, vec2 dir, float longi, float amp, float n){
	return vec3(amp*n*pow((sin(phi*t + (position.x*dir.x + position.z*dir.y)*longi)+1)/2,n-1)*cos(phi*t + (position.x*dir.x + position.z*dir.y)*longi)/2*dir.x*longi,
				1.0,
				amp*n*pow((sin(phi*t + (position.x*dir.x + position.z*dir.y)*longi)+1)/2,n-1)*cos(phi*t + (position.x*dir.x + position.z*dir.y)*longi)/2*dir.y*longi);
}

void main(){
	float totalHeight = (generateHeight(2,vec2(1,0),0.7,1,2.5) + generateHeight(1,vec2(-1,0),0.4,3,5.5) + generateHeight(1,vec2(0.5,0.5),0.28,5,5.5)
						+ generateHeight(1,vec2(-0.25,0.75),1,4,2.5) + generateHeight(1,vec2(0.25,-0.75),0.4,3,4.5) + generateHeight(2,vec2(-0.5,-0.5),0.6,2,3.5)) * amplitude;
	gl_Position =  worldTransform*vec4(position.x,position.y + totalHeight,position.z,1.0);
	normal = normalize((gradient(2,vec2(1,0),0.7,1,2.5) + gradient(1,vec2(-1,0),0.4,3,5.5) + gradient(1,vec2(0.5,0.5),0.28,5,5.5)
						+ gradient(1,vec2(-0.25,0.75),1,4,2.5) + gradient(1,vec2(0.25,-0.75),0.4,3,4.5) + gradient(2,vec2(-0.5,-0.5),0.6,2,3.5))*amplitude);
	passTextCoord = textureCoord + phi*(0.05,0.05)*0.8;
}