#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 normalToFrag;

uniform mat4 worldTransform;
uniform mat4 modelTransform;

void main(){
  gl_Position =  worldTransform * vec4(position,1.0);
  normalToFrag = (modelTransform * vec4(normal,0.0f)).xyz;
}