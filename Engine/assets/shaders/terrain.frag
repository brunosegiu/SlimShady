#version 330 core

in vec3 normalToFrag;
in vec2 textureCoordToFrag;

out vec4 outColor;

uniform sampler2D textSampler;

void main() {
	vec3 directional = normalize(vec3(0.0f,0.5f,1.0f)); 
	vec4 baseColor = texture(textSampler, textureCoordToFrag);// vec4(0.545098f,0.270588f,0.0745098,1.0f);
	float coef = dot(normalToFrag, directional);
	if (coef < 0.1f)
		coef = 0.1f;
	outColor = baseColor * coef;
}