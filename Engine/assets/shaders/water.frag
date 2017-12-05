#version 330 core

in vec3 normal;
in vec2 passTextCoord;

uniform vec3 lightdir;
uniform vec3 lightcolor;
uniform float intensity;
uniform vec3 moondir;
uniform vec3 mooncolor;
uniform float mintensity;
uniform vec3 cameraPos;
uniform sampler2D textSampler;
uniform sampler2D normalSampler;

//const vec3 watercolor = vec3(0.2,0.2,0.5);
const vec3 speccol = vec3(0.5,0.5,0.5);
const float reffactor = 0.5;
//const vec3 lightdirr = vec3(-1.0,-1.0,0);

layout(location = 0) out vec3 outColor;

vec3 specular(vec3 cameraPos, vec3 normal){
	vec3 view = normalize(gl_FragCoord.xyz-cameraPos);
	vec3 reflected = normalize((vec4(reflect(-lightdir, normal),1.0)).xyz);
	float factor = max(dot(reflected,view),0);
	return speccol*pow(factor,2)*reffactor;
}

void main() {
	vec4 textNormal = texture(normalSampler, passTextCoord);
	vec4 bNormal = vec4(normal, 1.0);
	bNormal = normalize(bNormal + textNormal*2-1);
	//vec3 specular = specular(cameraPos, bNormal.xyz);
	float factor = max(dot(-lightdir,bNormal.xyz),0.1);
	vec4 baseColor = texture(textSampler, passTextCoord);
	vec3 color = (baseColor.xyz*lightcolor*factor)*intensity;
	factor = max(dot(-moondir,bNormal.xyz),0.1);
	vec3 mcolor = (baseColor.xyz*mooncolor*factor)*mintensity;
	outColor = color + mcolor;
	//outColor += specular(cameraPos, bNormal.xyz);
}