#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform vec2 invScreenSize;
uniform bool on;

void main(){
	if (on){
		float FXAA_MAX_SAMPLE_DIST = 8.0f;
		float FXAA_REDUCE_MIN = 1.0f/128.0f;
		float FXAA_REDUCE_MUL = 1.0f/8.0f;

		vec3 luma = vec3(0.299,0.587,0.114);

		float lumaTL = dot(luma, texture(sampler, UV + vec2(-1.0f,-1.0f) * invScreenSize).xyz );
		float lumaTR = dot(luma, texture(sampler, UV + vec2(1.0f,-1.0f) * invScreenSize).xyz );
		float lumaBL = dot(luma, texture(sampler, UV + vec2(-1.0f,1.0f) * invScreenSize).xyz );
		float lumaBR = dot(luma, texture(sampler, UV + vec2(1.0f,1.0f) * invScreenSize).xyz );
		float lumaM = dot(luma, texture(sampler, UV).xyz );
	
		vec2 dir;
		dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
		dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));

		float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (FXAA_REDUCE_MUL*0.25f), FXAA_REDUCE_MIN);
		float normalizer = 1.0f/(min(abs(dir.x), abs(dir.y)) + dirReduce);
		dir = dir * normalizer;
	
		dir = clamp(dir, -FXAA_MAX_SAMPLE_DIST, FXAA_MAX_SAMPLE_DIST) * invScreenSize;

		vec3 res1 = 0.5f * (texture(sampler, UV + (dir * vec2(1.0f/3.0f - 0.5f))).xyz +
						texture(sampler, UV + (dir * vec2(2.0f/3.0f - 0.5f))).xyz );
		vec3 res2 = res1 * (0.5f) + (0.25f) * (texture(sampler, UV).xyz +
						texture(sampler, UV + (dir * vec2(0.5f)) ).xyz);
		float lumaMin = min(lumaM, min(lumaTL, min(lumaTR, min(lumaBL, lumaBR))));
		float lumaMax = max(lumaM, max(lumaTL, max(lumaTR, max(lumaBL, lumaBR))));
		float lumaRes2 = dot(luma, res2);
		if (lumaRes2 < lumaMin || lumaRes2 > lumaMax){
			color = res1;
		}else{
			color = res2;
		}
	}else{
		color = texture(sampler, UV).xyz;
	}
}