#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform vec2 invScreenSize;
uniform bool on;

void main(){
	if (on)
		color = (1.0f,1.0f,1.0f) - (texture(sampler, UV).xyz);
	else 
		color = texture(sampler, UV).xyz;
}