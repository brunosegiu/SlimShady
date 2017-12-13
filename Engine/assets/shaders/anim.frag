// Written by Karl Zylinski
// Portfolio and website: http://zylinski.se
// Use, modify and copy in any way you want

#version 330

in vec3 normal;
in vec2 texCoord;

uniform sampler2D sampler;
uniform vec3 lightdir;
uniform vec3 lightcolor;
uniform vec3 moondir;
uniform vec3 mooncolor;

layout(location = 0) out vec4 outColor;

void main(){
	vec4 baseColor;
	baseColor = texture(sampler, texCoord);
	if (baseColor.a < 1.0f)
		discard;
	float coef = dot(normal,-lightdir);
	if (coef < 0.1f)
		coef = 0.1f;
	float coefm = dot(normal,-moondir);
	if (coefm < 0.1f)
		coefm = 0.1f;
	outColor = vec4(baseColor.rgb * (lightcolor * coef + mooncolor * coefm),1.0f);
}