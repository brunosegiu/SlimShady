#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in float mass[];
out vec4 color;

uniform mat4 worldTransform;
uniform float maxMass;

void main(){
	float factor = mass[0]/maxMass; 

	gl_Position = gl_in[0].gl_Position;
	color = vec4(1.0,1.0,1.0,1.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + worldTransform*vec4(0.0,5*factor,0.0,0.0);
	color = vec4(1.0,1.0,1.0,0.0);
	EmitVertex();

	EndPrimitive();
}