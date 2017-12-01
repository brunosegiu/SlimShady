#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform sampler2D samplerDB;
uniform float gamma;
uniform float contrast;
uniform float brightness;

float linearize(float depth){
	return (2 * 0.5)/ (5000.0f + 0.5 - depth * (5000.0f-0.5));
}

void main(){
		//Correccion de color
		color = texture(sampler, UV).xyz;
		color = vec3(pow(color.r,1.0/gamma),pow(color.g,1.0/gamma),pow(color.b,1.0/gamma));
		color = color * contrast + vec3(brightness);

		//FOG
		float depth = texture(samplerDB,UV).r;
		depth = linearize(depth);
		float factor = pow(depth, 0.3f);
		color = color * (1-factor) + vec3(factor);

}