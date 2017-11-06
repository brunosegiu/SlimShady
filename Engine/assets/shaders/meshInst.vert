#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in vec3 instancePos;

out vec3 normalToFrag;
out vec2 textCoordFrag;

uniform mat4 worldTransform;
uniform mat4 modelTransform;

void main(){
  vec3 aux = position+instancePos;
  gl_Position =  worldTransform * vec4(aux,1);
  normalToFrag = (modelTransform * vec4(normal,0)).xyz;
  textCoordFrag = textureCoord;
}