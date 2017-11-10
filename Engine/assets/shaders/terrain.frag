#version 330 core

in vec3 normalToFrag;
in vec2 textureCoordToFrag;

out vec4 outColor;

uniform sampler2D textSampler;
uniform vec3 lightdir;
uniform vec3 lightcolor;

void main() {
	vec4 baseColor = texture(textSampler, textureCoordToFrag);
	float coef = dot(normalToFrag, -lightdir);
	if (coef < 0.1f)
		coef = 0.1f;
	outColor = baseColor * lightcolor * coef;
}