//Version number
#version 400

//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;

//Taking in the Texture coordinates
in vec2 TexCoords; 

uniform sampler2D diffuse;

void main()
{
	fragcolor = texture2D(diffuse, TexCoords);
}






