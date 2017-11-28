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

float generateHeight(){
	float wave1 = pow((sin(phi*2 + position.x/3)+1)/2,2.5);
	float wave2 = pow((sin(phi + (position.x*0.5+position.z*0.5)/7)+1)/2,5.5)*5;
	//float wave3 = pow((sin(phi + (position.x*0.75+position.z*0.25)/16)+1)/2,5.5)*3;
	float wave3 = pow((sin(phi - (position.x*0.25-position.z*0.75)/2)+1)/2,2.5)*4;
	float wave4 = pow((sin(phi + (position.x*0.25-position.z*0.75)/5)+1)/2,4.5)*3;
	float wave5 = pow((sin(phi - position.x/5)+1)/2,5.5)*3;
	float wave6 = pow((sin(phi*2 - (position.x*0.5+position.z*0.5)/3)+1)/2,3.5)*2;
	return (wave1+wave2+wave3+wave4+wave5+wave6)*amplitude;
}

vec3 gradient(){ //F(x,y,x) = y - f(x,z)
	vec3 wave1 = vec3(cos(phi*2 + position.x/3)/3*amplitude * 2.5*pow((sin(phi*2 + position.x/3)+1)/2,1.5), 0.3, 0.0);
	vec3 wave2 = vec3(cos(phi + (position.x*0.5+position.z*0.5)/7)/7*0.5*amplitude*5 * 5.5*pow((sin(phi + (position.x*0.5+position.z*0.5)/7)+1)/2,4.5), 0.3,
						cos(phi + (position.x*0.5+position.z*0.5)/7)/7*0.5*amplitude*5 * 5.5*pow((sin(phi + (position.x*0.5+position.z*0.5)/7)+1)/2,4.5));
	//vec3 wave3 = vec3(cos(phi + (position.x*0.75+position.z*0.25)/16)/16*0.75*amplitude*3 * 5.5*pow((sin(phi + (position.x*0.75+position.z*0.25)/16)+1)/2,4.5), 0.3,
		//				cos(phi + (position.x*0.75+position.z*0.25)/16)/16*0.25*amplitude*3 * 2.5*pow((sin(phi + (position.x*0.75+position.z*0.25)/16)+1)/2,1.5));
	vec3 wave3 = vec3(cos(phi - (position.x*0.25-position.z*0.75)/2)/2*-0.25*amplitude*4 * 5.5*pow((sin(phi - (position.x*0.25-position.z*0.75)/2)+1)/2,4.5), 0.3,
						cos(phi - (position.x*0.25-position.z*0.75)/2)/2*0.75*amplitude*4 * 2.5*pow((sin(phi - (position.x*0.25-position.z*0.75)/2)+1)/2,1.5));
	vec3 wave4 = vec3(cos(phi + (position.x*0.25-position.z*0.75)/5)/5*0.25*amplitude*3 * 4.5*pow((sin(phi + (position.x*0.25-position.z*0.75)/5)+1)/2,3.5), 0.3,
						cos(phi + (position.x*0.25-position.z*0.75)/5)/5*-0.75*amplitude*3 * 4.5*pow((sin(phi + (position.x*0.25-position.z*0.75)/5)+1)/2,3.5));
	vec3 wave5 = vec3(cos(phi - position.x/5)/-5*amplitude*3 * 5.5*pow((sin(phi - position.x/5)+1)/2,4.5), 0.3, 0.0);
	vec3 wave6 = vec3(cos(phi*2 - (position.x*0.5+position.z*0.5)/3)/3*0.5*amplitude*2 * 3.5*pow((sin(phi*2 - (position.x*0.5+position.z*0.5)/3)+1)/2,2.5), 0.3,
						cos(phi*2 - (position.x*0.5+position.z*0.5)/3)/3*0.5*amplitude*2 * 3.5*pow((sin(phi*2 - (position.x*0.5+position.z*0.5)/3)+1)/2,2.5));
	return normalize(wave1+wave2+wave3+wave4+wave5+wave6);
}

void main(){
  gl_Position =  worldTransform*vec4(position.x,position.y + generateHeight(),position.z,1.0);
  normal = gradient();
  passTextCoord = textureCoord + phi*(0.05,0.05)*0.8;
}