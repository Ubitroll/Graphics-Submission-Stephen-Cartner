#version 400

//Bringing in stored attributes
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextureCords;
layout (location = 2) in vec3 VertexNormal;

//Passing variables out
out VS_OUT {
    vec2 texCords;
	vec3 vertPos;
	vec3 vertNormal;
	
} vs_out;

//Bringing in uniform varibles
uniform mat4 transform;

void main()
{
	//Storing data to be passed out
	vs_out.vertPos = VertexPosition;
	vs_out.texCords = TextureCords;
	vs_out.vertNormal = vec3(transform * vec4(VertexNormal, 0.0));

	//Updating the positions
	gl_Position = transform * vec4(VertexPosition, 1.0);
}