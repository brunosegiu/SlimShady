#version 330 core

in vec2 passTextCoord;

uniform vec3 lightcolor;
uniform float intensity;
uniform vec3 mooncolor;
uniform float mintensity;
//uniform vec3 cameraPos;
uniform sampler2D textSampler;

layout(location = 0) out vec3 outColor;

void main() {
	vec4 baseColor = texture(textSampler, passTextCoord);
	vec3 color = (baseColor.xyz*lightcolor)*intensity;
	vec3 mcolor = (baseColor.xyz*mooncolor)*mintensity;
	outColor = color + mcolor;
}