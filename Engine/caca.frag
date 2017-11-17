#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform vec2 invScreenSize;
uniform bool on;

void main(){
	color = vec3(texture(sampler, UV).x, 0, 0);
}