#version 330 core

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

uniform bool isTextured;
uniform sampler2D textSampler;
uniform vec3 Kd;

layout (location = 0) out vec3 worldPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec3 texCoordOut;

void main(){
    worldPosOut = worldPos0;
	if (isTextured){
	    diffuseOut = texture(textSampler, texCoord0).xyz;
	}else{
		diffuseOut = Kd;
	}
    normalOut = normalize(normal0);
    texCoordOut = vec3(texCoord0, 0.0);
} 