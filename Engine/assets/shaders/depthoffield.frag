#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform sampler2D samplerDB;
uniform bool on;

const float blurclamp = 0.1;

void addDiff(vec2 offset){
	color += texture(sampler, UV + offset).rgb;
}

float linearize(float depth){
	return (2 * 0.5)/ (5000.0f + 0.5 - depth * (5000.0f-0.5));
}

void main(){
	if (on){
		float focus = texture(samplerDB, vec2(0.5)).r;
		float depth = texture(samplerDB,UV).r;
		focus = linearize(focus);
		depth = linearize(depth);

		float diff = abs(depth - focus);

		float aspectRatio = 1920/1080.0f;
		vec2 aspectCorrect = vec2(1.0,aspectRatio);

		vec2 blur = vec2(clamp(diff, -blurclamp, blurclamp));

		color = texture(sampler, UV).xyz;

		vec2 mask[40];
		mask[0] = vec2(0.0,0.4);
		mask[1] = vec2(0.15,0.37);
		mask[2] = vec2(0.29,0.29);
		mask[3] = vec2(-0.37,0.15);
		mask[4] = vec2(0.4,0.0);
		mask[5] = vec2(0.37,-0.15);
		mask[6] = vec2(0.29,-0.29);
		mask[7] = vec2(-0.15,-0.37);
		mask[8] = vec2(0.0,-0.4);
		mask[9] = vec2(-0.15,0.37);
		mask[10] = vec2(-0.29,0.29);
		mask[11] = vec2(0.37,0.15);
		mask[12] = vec2(0.4,0.0);
		mask[13] = vec2(-0.37,-0.15);
		mask[14] = vec2(-0.29,-0.29);
		mask[15] = vec2(0.15,-0.37);

		mask[16] = vec2(0.15,0.37);
		mask[17] = vec2(-0.37,0.15);
		mask[18] = vec2(0.37,-0.15);
		mask[19] = vec2(-0.15,-0.37);
		mask[20] = vec2(-0.15,0.37);
		mask[21] = vec2(0.37,0.15);
		mask[22] = vec2(-0.37,0.15);
		mask[23] = vec2(0.15,-0.37);

		mask[24] = vec2(0.29,0.29);
		mask[25] = vec2(0.4,0.0);
		mask[26] = vec2(0.29,-0.29);
		mask[27] = vec2(0.0,-0.4);
		mask[28] = vec2(-0.29,0.29);
		mask[29] = vec2(-0.4,0.0);
		mask[30] = vec2(-0.29,-0.29);
		mask[31] = vec2(0.0,0.4);

		mask[32] = vec2(0.29,0.29);
		mask[33] = vec2(0.4,0.0);
		mask[34] = vec2(0.29,-0.29);
		mask[35] = vec2(0.0,-0.4);
		mask[36] = vec2(-0.29,0.29);
		mask[37] = vec2(-0.4,0.0);
		mask[38] = vec2(-0.29,-0.29);
		mask[39] = vec2(0.0,0.4);
		float fact = 1.0f;
		float bias2 = 0.1f;
		for (int i = 0; i < 40; i++){
			if (i > 15)
				bias2 = 0.09f * fact;
			if (i > 23)
				bias2 = 0.07 * fact;
			if (i > 31)
				bias2 = 0.04 * fact;
			addDiff(mask[i] * aspectCorrect * blur * bias2);
		}
		color = color / 41.0f;
	}else{
		color = texture(sampler, UV).xyz;
	}
}