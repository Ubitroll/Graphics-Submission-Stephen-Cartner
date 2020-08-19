#pragma once
#include <string>
#include <GL\glew.h>
#include <vector>

class Texture
{
public:
	Texture();

	void Init(const std::string& filename);
	void Bind(unsigned int unit); // bind upto 32 textures

	GLuint loadCubemap(std::vector<std::string> faces);

	~Texture();

protected:
private:

	GLuint textureHandler;
};

