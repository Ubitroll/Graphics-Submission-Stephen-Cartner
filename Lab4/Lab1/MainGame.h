#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
//#include "Audio.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	//void playAudio(unsigned int Source, glm::vec3 pos);

	void setUpShaderReflection();
	void setUpShaderExplosion();
	void setUpShaderExtension();

	void SkyBox();

	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;

	Display _gameDisplay;
	GameState _gameState;
	Mesh meshEnviroment;
	Mesh meshExplosion;
	Mesh meshExtension;
	Camera myCamera;

	Texture texture;
	Texture textureExplosion;

	Texture textureSkybox;
	
	Shader shaderReflection;
	Shader shaderExplosion;
	Shader shaderExtension;

	Shader shaderSkybox;

	//Audio audioDevice;

	float counter;
	//unsigned int whistle;
	//unsigned int backGroundMusic;
};

