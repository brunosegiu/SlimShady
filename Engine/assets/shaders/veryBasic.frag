#version 330 core

in vec3 color;

out vec4 outColor;

const vec3 lightdir = normalize(vec3(-1.0,-1.0,0.0));
const vec3 watercolor = vec3(0.2,0.5,0.8);

void main() {
	//float factor = max(dot(normal,-lightdir),0.3);
	//vec3 color = watercolor*factor;
	outColor= vec4(color,1.0);
}