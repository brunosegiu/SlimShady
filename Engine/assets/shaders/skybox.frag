#version 330 core

in vec2 passTextCoord;

uniform vec3 lightcolor;
uniform float intensity;
uniform vec3 mooncolor;
uniform float mintensity;
uniform float phi;
//uniform vec3 cameraPos;
uniform sampler2D textSampler1;
uniform sampler2D textSampler2;

layout(location = 0) out vec3 outColor;

void main() {
	vec4 baseColor1 = texture(textSampler1, passTextCoord);
	vec4 baseColor2 = texture(textSampler2, passTextCoord);
	vec4 baseColor = (1-intensity)*baseColor2 + intensity*baseColor1;
	vec3 color = (baseColor.xyz*lightcolor)*intensity;
	vec3 mcolor = (baseColor.xyz*mooncolor)*mintensity;
	outColor = color + mcolor;
}