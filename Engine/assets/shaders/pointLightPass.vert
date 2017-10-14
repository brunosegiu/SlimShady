#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

uniform mat4 worldTransform;
uniform mat4 modelTransform;

void main(){
  gl_Position =  worldTransform * vec4(position,1);
  texCoord0 = textureCoord;
  normal0 = vec3(vec4(normal, 0.0f) * modelTransform).xyz;
  worldPos0 = vec3(vec4(position, 1.0f) * modelTransform).xyz;
}