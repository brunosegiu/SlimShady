#version 330 core

in vec3 normal;
in vec2 passTextCoord;

uniform vec3 lightdir;
uniform vec3 lightcolor;
//uniform vec3 cameraPos;
uniform sampler2D textSampler;

const vec3 watercolor = vec3(0.2,0.2,0.5);
//const vec3 speccol = vec3(1.0,1.0,1.0);
//const float reffactor = 0.5;
const vec3 lightdirr = vec3(-1.0,-1.0,0);

out vec4 outColor;

//vec3 specular(vec3 cameraPos, vec3 normal){
	//vec3 view = normalize(cameraPos - gl_Position.xyz);
	//vec3 reflected = normalize((worldTransform*vec4(reflect(-lightdir, normal),1.0)).xyz);
	//float factor = max(dot(reflected,view),0);
	//return speccol*pow(factor,15)*reffactor;
//}

void main() {
	//vec3 specular = specular(cameraPos, normal);
	float factor = max(dot(-lightdir,normal),0.3);
	vec4 baseColor = texture(textSampler, passTextCoord);
	vec3 color = (baseColor.xyz*lightcolor*factor);
	outColor= vec4(color,1.0);
}