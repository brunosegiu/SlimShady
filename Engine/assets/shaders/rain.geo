#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

out vec4 color;

void main(){
	
	gl_Position = gl_in[0].gl_Position;
	//gl_PointSize = 10.0;
	color = vec4(1.0,1.0,1.0,0.5);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.0,3.0,0.0,0.0);
	color = vec4(1.0,1.0,1.0,0.0);
	EmitVertex();

	EndPrimitive();
}