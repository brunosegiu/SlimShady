#version 330 core

layout(location = 0) out vec3 outColor;

in vec2 textCoordFrag;
in mat3 toModelSpace;

uniform sampler2D textSampler;
uniform sampler2D normTextSampler;
uniform vec3 lightdir;
uniform vec3 lightcolor;
uniform vec3 moondir;
uniform vec3 mooncolor;

void main() {
	vec4 baseColor = texture(textSampler, textCoordFrag);
	vec3 normal = normalize( toModelSpace * ((2*texture(normTextSampler, textCoordFrag)-1).xyz));
	float coef = dot(normal, -lightdir);
	if (coef < 0.1f)
		coef = 0.1f;
	float coefm = dot(normal,-moondir);
	if (coefm < 0.1f)
		coefm = 0.1f;
	outColor = baseColor.rgb * (lightcolor * coef + mooncolor * coefm);
}