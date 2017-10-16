#version 330 core

layout (location = 0) in vec3 position;

out vec3 color;

uniform mat4 worldTransform;

void main(){
  gl_Position =  worldTransform * vec4(position,1.0);
  color = vec3(1.0,1.0,1.0);
}