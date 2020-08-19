#version 330 core

//Bringing in stored attributes
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

//passing out variables
out vec3 Normal;
out vec3 Position;

//Bringing in uniform variables
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  