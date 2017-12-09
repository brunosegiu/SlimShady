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

vec4 blur13(sampler2D image, vec2 uv, vec2 resolution) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353);
  vec2 off2 = vec2(3.2941176470588234);
  vec2 off3 = vec2(5.176470588235294);
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
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
			color = color * (1-blurStr) + blur13(samplerOld, UV, vec2(1600,900)).xyz * blurStr;
		}
}