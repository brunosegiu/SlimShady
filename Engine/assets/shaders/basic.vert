#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

out vec3 normalToFrag;
out vec4 baseColor;

uniform mat4 worldTransform;
uniform vec3 Kd;
uniform bool isTextured;
uniform sampler2D textSampler;

void main(){
  gl_Position =  worldTransform * vec4(position,1);
  if (isTextured){
	baseColor = texture(textSampler, textureCoord);
  }else{
	baseColor = vec4(Kd,1);
  }
  normalToFrag = normal;
}