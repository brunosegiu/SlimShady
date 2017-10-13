#version 330 core

uniform sampler2D tex;

in vec3 normalToFrag;
in vec2 textCoord;
in vec4 baseColor;

out vec4 color;

void main() {
	vec3 directional = vec3(1.0f,0.0f,0.0f); 
	color = baseColor * dot(normalToFrag, directional);
}