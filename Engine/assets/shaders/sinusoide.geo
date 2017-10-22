#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 5) out;

uniform mat4 worldTransform;

in vec3 normal[];
out vec3 color;

const vec3 lightdir = normalize(vec3(-1.0,-1.0,0.0));
const vec3 watercolor = vec3(0.0,0.0,1.0);

vec3 baricentro(vec3 A, vec3 B, vec3 C){
	return(vec3((A.x+B.x+C.x)*0.33, (A.y+B.y+C.y)*0.33, (A.z+B.z+C.z)*0.33));
}

vec3 calculateBarNormal(){
	return normalize(normal[0]*0.33+normal[1]*0.33+normal[2]*0.33);
}

void main(){
	vec3 bnormal = calculateBarNormal();
	vec3 baricentro = baricentro(gl_in[0].gl_Position.xyz,gl_in[1].gl_Position.xyz,gl_in[2].gl_Position.xyz);
	float factor;
	
	gl_Position =  worldTransform*gl_in[0].gl_Position;
	factor = max(dot(-lightdir,normal[0]),0.3);		//No hace falta cambiar coordenadas, el producto es lo mismo
	color = watercolor*factor;
	EmitVertex();
	
	gl_Position = worldTransform*gl_in[1].gl_Position;
	factor = max(dot(-lightdir,normal[1]),0.3);
	color = watercolor*factor;
	EmitVertex();

	gl_Position = worldTransform*vec4(baricentro.xyz,1.0);
	factor = max(dot(-lightdir,bnormal),0.3);
	color = watercolor*factor;
	EmitVertex();
	
	gl_Position = worldTransform*gl_in[2].gl_Position;
	factor = max(dot(-lightdir,normal[2]),0.3);
	color = watercolor*factor;
	EmitVertex();

	gl_Position = worldTransform*gl_in[0].gl_Position;
	factor = max(dot(-lightdir,normal[0]),0.3);
	color = watercolor*factor;
	EmitVertex();
	
	EndPrimitive();
}