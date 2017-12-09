#version 330 core

in vec3 normalToFrag;
in vec2 textureCoordToFrag;

layout(location = 0) out vec4 outColor;

//out vec4 outColor;

uniform sampler2D blendMapSampler;
uniform sampler2D texture1Sampler; //r
uniform sampler2D texture2Sampler; //g
uniform sampler2D texture3Sampler; //b
uniform sampler2D texture4Sampler; //0

uniform vec3 lightdir;
uniform vec3 lightcolor;
uniform vec3 moondir;
uniform vec3 mooncolor;

void main() {
	vec3 blend = texture(blendMapSampler, textureCoordToFrag).xyz;
	float fourth = 1.0f - (blend.x+blend.y+blend.z);
	vec2 tiledCoords = textureCoordToFrag * 40.0f;
	vec3 baseColor = (texture(texture1Sampler, tiledCoords) * blend.x + texture(texture2Sampler, tiledCoords) * blend.y + texture(texture3Sampler, tiledCoords) * blend.z + texture(texture4Sampler, tiledCoords) * fourth).xyz;
	float coef = dot(normalToFrag, -lightdir);
	if (coef < 0.1f)
		coef = 0.1f;
	float coefm = dot(normalToFrag,-moondir);
	if (coefm < 0.1f)
		coefm = 0.1f;
	outColor = vec4(baseColor.rgb * (lightcolor * coef + mooncolor * coefm),1.0f);
}