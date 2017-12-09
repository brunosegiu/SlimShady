#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D sampler;
uniform sampler2D samplerOld;
uniform sampler2D samplerDB;
uniform float gamma;
uniform float contrast;
uniform float brightness;
uniform float fogFactor;
uniform float vignette;
uniform float blurStr;
uniform bool firstPass;


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
		float factor = clamp(1.0f/exp(depth* fogFactor),0.0f,1.0f);
		color = color * (factor) + vec3(1.0f-factor);

		//Vignette
		vec2 vig = UV * (1.0f-UV.yx);
		float eff = pow(vig.x*vig.y*15.0f, vignette);
		color = clamp(color, 0.0f, 1.0f);
		color = color * eff;
		
		if (!firstPass){
			color = color * (1-blurStr) + blurStr * texture(samplerOld, UV).xyz;// color * 0.5 + 0.5  * ;
		}
}