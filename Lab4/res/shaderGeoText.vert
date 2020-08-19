//Version Number
#version 400

//bringing in stored attributes
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 VertexNormal;

//Passing out texCoords
out VS_OUT {
    vec2 texCoords;
} vs_out;

//Uniform variable
uniform mat4 transform;

void main()
{

	vs_out.texCoords = TexCoords;

	// Sets the position of the current vertex
	gl_Position = transform * vec4(VertexPosition, 1.0);
}