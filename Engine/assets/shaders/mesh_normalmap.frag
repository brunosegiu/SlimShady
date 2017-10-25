#version 330 core

out vec4 color;

in vec2 textCoordFrag;
in vec3 normalToFrag;
in vec3 tangentToFrag;
in vec3 bitangentToFrag;
in mat3 toModelSpace;

uniform sampler2D textSampler;
uniform sampler2D normTextSampler;

void main() {
	vec3 directional = vec3(0.0f,0.0f,1.0f); 
	vec4 baseColor = texture(textSampler, textCoordFrag);
	vec3 normal = normalize( toModelSpace * ((2*texture(normTextSampler, textCoordFrag)-1).xyz));
	float coef = dot(normal, directional);
	if (coef <= 0.1f)
		coef = 0.1;
	color = baseColor * coef;
}