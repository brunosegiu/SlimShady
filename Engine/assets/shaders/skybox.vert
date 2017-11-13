#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 worldTransform;
//uniform float phi;

//const float PI = 3.1415926535897932384626433832795;
//const float amplitude = 0.5;
//const vec3 watercolor = vec3(0.2,0.2,0.5);

//out vec3 normal;
out vec2 passTextCoord;

void main(){
  gl_Position =  worldTransform*vec4(position.xyz,1.0);
  passTextCoord = textureCoord;
}