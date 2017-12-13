#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;

layout(location = 0)  out vec4 outputColor;

uniform sampler2D gSampler;

uniform vec3 vEyePosition;
uniform vec3 lightdir;
uniform vec3 lightcolor;
uniform vec3 moondir;
uniform vec3 mooncolor;

//uniform Material matActive;

uniform float fAlphaTest;
uniform float fAlphaMultiplier;

void main()
{
	vec4 vTexColor = texture2D(gSampler, vTexCoord);
	float fNewAlpha = vTexColor.a*fAlphaMultiplier;               
	if(fNewAlpha < fAlphaTest)
		discard;
	
	if(fNewAlpha > 1.0f)
		fNewAlpha = 1.0f;	
	vec3 normal = normalize(-vWorldPos + vEyePosition);
	vec4 baseColor = vTexColor; 
	float coef = dot(normal,-lightdir);
	if (coef < 0.1f)
		coef = 0.1f;
	float coefm = dot(normal,-moondir);
	if (coefm < 0.1f)
		coefm = 0.1f;
	vec4 outColor = vec4(baseColor.zyx * (lightcolor * coef + mooncolor * coefm),1.0f);
	outputColor = vec4(baseColor.xyz, fNewAlpha);
}