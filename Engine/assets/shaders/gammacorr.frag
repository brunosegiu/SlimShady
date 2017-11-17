#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform sampler2D samplerDB;
uniform vec2 invScreenSize;
uniform bool on;
uniform float gamma;

void main(){
	//if (on)
		float depth = texture(samplerDB,UV).r;
		color = vec3(depth); //samplerDB (1.0f,1.0f,1.0f) - (texture(sampler, UV).xyz);
	//else 
		color = texture(sampler, UV).xyz;
		color = vec3(pow(color.r,gamma),pow(color.g,gamma),pow(color.b,gamma));
}