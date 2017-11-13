#version 330 core

out vec4 outColor;

in vec2 textCoordFrag;
in vec3 normalToFrag;
in vec3 tangentToFrag;
in vec3 bitangentToFrag;
in mat3 toModelSpace;

uniform sampler2D textSampler;
uniform sampler2D normTextSampler;
uniform vec3 lightdir;
uniform vec3 lightcolor;

void main() {
	vec4 baseColor = texture(textSampler, textCoordFrag);
	vec3 normal = normalize( toModelSpace * ((2*texture(normTextSampler, textCoordFrag)-1).xyz));
	float coef = dot(normal, -lightdir);
	if (coef < 0.3f)
		coef = 0.3f;
	outColor = vec4(baseColor.xyz * lightcolor * coef,1.0f);
}