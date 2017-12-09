#version 330 core

layout(location = 0) out vec4 outColor;

in vec4 color;

void main() {
    outColor = color.rgba;
	//outColor = vec3(1.0,1.0,1.0);
}