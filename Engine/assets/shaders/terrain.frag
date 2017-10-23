#version 330 core

in vec3 normalToFrag;
in vec2 textureCoordToFrag;

out vec4 outColor;

uniform sampler2D textSampler;

void main() {
	vec3 directional = normalize(vec3(0.0f,0.8f,1.0f)); 
	vec4 baseColor = texture(textSampler, textureCoordToFrag);
	float coef = dot(normalToFrag, directional);
	if (coef < 0.1f)
		coef = 0.1f;
	outColor = baseColor * coef;
}