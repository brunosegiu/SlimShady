#version 330 core

in vec3 normalToFrag;
in vec2 textCoordFrag;

out vec4 color;

uniform bool isTextured;
uniform sampler2D textSampler;
uniform vec3 lightdir;
uniform vec3 lightcolor;

void main() {
	vec4 baseColor;
	baseColor = texture(textSampler, textCoordFrag);
	if (baseColor.a < 0.5f)
		discard;
	float coef = dot(normalToFrag, lightdir);
	if (coef < 0.05f)
		coef = 0.05f;
	color = baseColor * lightcolor * coef;
}