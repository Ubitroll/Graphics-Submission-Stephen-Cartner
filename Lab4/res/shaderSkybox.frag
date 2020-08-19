#version 330 core

//passing out the Frag Colour
out vec4 FragColor;

//taking in texCoords
in vec3 TexCoords;

//Bringing in uniform variables
uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}