#version 330 core

in vec3 normalToFrag;
in vec2 textCoordFrag;

layout(location = 0) out vec3 outColor;

uniform bool isTextured;
uniform sampler2D textSampler;
uniform vec3 lightdir;
uniform vec3 lightcolor;
uniform vec3 moondir;
uniform vec3 mooncolor;

void main() {
	vec4 baseColor;
	baseColor = texture(textSampler, textCoordFrag);
	if (baseColor.a < 1.0f)
		discard;
	float coef = dot(normalToFrag,-lightdir);
	if (coef < 0.3f)
		coef = 0.3f;
	float coefm = dot(normalToFrag,-moondir);
	if (coefm < 0.1f)
		coefm = 0.1f;
	outColor = baseColor.rgb * (lightcolor * coef + mooncolor * coefm);
}