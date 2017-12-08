// Written by Karl Zylinski
// Portfolio and website: http://zylinski.se
// Use, modify and copy in any way you want

#version 330

in vec3 normal;
in vec2 texCoord;

layout(location = 0) out vec3 outColor;

// Animation-wise therse is nothing special about the fragment shader, it has texture and lightning-support
void main(){
	outColor = vec3(1.0f);
}