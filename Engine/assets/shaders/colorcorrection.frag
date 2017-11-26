#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform sampler2D samplerDB;
uniform float gamma;
uniform float contrast;
uniform float brightness;

void main(){
		/*float depth = texture(samplerDB,UV).r;
		color = vec3(depth); //samplerDB (1.0f,1.0f,1.0f) - (texture(sampler, UV).xyz);*/
		color = texture(sampler, UV).xyz;
		color = vec3(pow(color.r,gamma),pow(color.g,gamma),pow(color.b,gamma));
		color = color * contrast + vec3(brightness);
}