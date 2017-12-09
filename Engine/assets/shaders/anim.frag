// Written by Karl Zylinski
// Portfolio and website: http://zylinski.se
// Use, modify and copy in any way you want

#version 330

in vec3 normal;
in vec2 texCoord;

layout(location = 0) out vec4 outColor;

// Animation-wise therse is nothing special about the fragment shader, it has texture and lightning-support
void main(){
	outColor = vec4(1.0f);
}