#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in vec3 tangent;

out vec2 textCoordFrag;
out mat3 toModelSpace;

uniform mat4 worldTransform;
uniform mat4 modelTransform;
uniform mat3 modelView;

void main(){
  gl_Position =  worldTransform * vec4(position,1);
  textCoordFrag =  textureCoord;
  vec3 normalToFrag = normalize((modelTransform * vec4(normal,0.0f)).xyz);
  vec3 tangentToFrag = normalize((modelTransform * vec4(tangent,0.0f)).xyz);
  vec3 bitangentToFrag = normalize(cross(tangentToFrag, normalToFrag));
  toModelSpace = inverse(mat3(tangentToFrag.x, bitangentToFrag.x ,normal.x,
					    tangentToFrag.y, bitangentToFrag.y ,normal.y,
				        tangentToFrag.z, bitangentToFrag.z ,normal.z));	
}