#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform float gamma;
uniform float contrast;
uniform float brightness;

void main(){
		//Correccion de color
		color = texture(sampler, UV).xyz;
		color = vec3(pow(color.r,1.0/gamma),pow(color.g,1.0/gamma),pow(color.b,1.0/gamma));
		color = color * contrast + vec3(brightness);
}