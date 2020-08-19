#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader()
{
	
}

void Shader::Init(const std::string& vertFile, const std::string& fragFile, const std::string& geoFile)
{
	//Creating the shader program
	program = glCreateProgram(); 

	//Storing the vertex, geom and frag shaders if provided
	if(vertFile != "")
		shaderVert = CreateShader(LoadShader("..\\res\\" + vertFile + ".vert"), GL_VERTEX_SHADER);
	if(geoFile != "")
		shaderGeo = CreateShader(LoadShader("..\\res\\" + geoFile + ".geom"), GL_GEOMETRY_SHADER);
	if(fragFile != "")
		shaderFrag = CreateShader(LoadShader("..\\res\\" + fragFile + ".frag"), GL_FRAGMENT_SHADER);

	//Attaching the shaders to the program
	glAttachShader(program, shaderVert);
	glAttachShader(program, shaderGeo);
	glAttachShader(program, shaderFrag);

	//Binding Attributes to locations to be accessed within the vert, frag or geom shader 
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "normals");

	//Linkining shader program and checking for error
	glLinkProgram(program); 
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed");

	//Checking if program is valid and error checking
	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform"); 
}


Shader::~Shader()
{

	//Detaching vert, frag and geom shader, then deleting them
	glDetachShader(program, shaderVert);
	glDeleteShader(shaderVert);

	glDetachShader(program, shaderFrag);
	glDeleteShader(shaderVert);

	glDetachShader(program, shaderGeo);
	glDeleteShader(shaderVert);

	//Deleting the program
	glDeleteProgram(program);
}

void Shader::Bind()
{
	//uses the program
	glUseProgram(program); 
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

