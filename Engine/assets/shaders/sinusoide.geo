#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vcolor[];
out vec3 color;

const vec3 lightdir = normalize(vec3(-1.0,-1.0,0.0));

vec3 calculateNormal(){
	vec3 v0v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 v0v2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = cross(v0v1,v0v2);
	return normalize(normal);
}

void main(){
	vec3 normal = calculateNormal();
	float factor = max(dot(-lightdir,normal),0);
	
	gl_Position = gl_in[0].gl_Position;
	color = vcolor[0]*factor;
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position;
	color = vcolor[1]*factor;
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position;
	color = vcolor[2]*factor;
	EmitVertex();
	
	EndPrimitive();
}