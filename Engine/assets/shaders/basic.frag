#version 330 core

in vec3 normalToFrag;
in vec2 textCoordFrag;

out vec4 color;

uniform bool isTextured;
uniform sampler2D textSampler;
uniform vec3 Kd;

void main() {
	vec3 directional = vec3(0.0f,0.0f,1.0f); 
	vec4 baseColor;
	if (isTextured){
	    baseColor = texture(textSampler, textCoordFrag);
		if (baseColor.a < 0.5f)
			discard;
	}else{
		baseColor = vec4(Kd,1);
	}
	float coef = dot(normalToFrag, directional);
	if (coef < 0.05f)
		coef = 0.05f;
	color = baseColor * coef;
}