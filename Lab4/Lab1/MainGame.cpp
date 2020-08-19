#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); 

	//Creating required Meshs 
    Mesh* meshEnviroment();
	Mesh* meshExplosion();
	Mesh* meshExtension();

	//Creating required shaders 
	Shader* shaderReflection();
	Shader* shaderExplosion();
	Shader* shaderExtension();
	Shader* shaderSkyBox();

	//Creating required textures 
	Texture* textureSkyBox();
	Texture* texture(); 
	Texture* textureExplosion();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	//Starting the game Display
	_gameDisplay.initDisplay(); 
	
	//Loading in models
	meshEnviroment.loadModel("..\\res\\monkey3.obj");
	meshExplosion.loadModel("..\\res\\sphere.obj");
	meshExtension.loadModel("..\\res\\Skull.obj");

	
	//Loading in textures
	texture.Init("water"); 
	textureExplosion.Init("Moss");

	//Loading in the vert, frag or geo shaders
	shaderReflection.Init("shaderReflection", "shaderReflection", "");
	shaderExplosion.Init("shaderGeoText", "shaderGeoText", "shaderGeoText");
	shaderExtension.Init("shaderExtension", "shaderExtension", "shaderExtension");
	shaderSkybox.Init("shaderSkyBox", "shaderSkyBox", "");
	
	//Starting Camera
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	//Setting up counter
	counter = 1.0f;

	//Storing the location of the faces used in skybox
	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	//Loading in faces using a cubemap and storing the textures
	cubemapTexture = textureSkybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	//Storing the Vertices used for the skybox
	float skyboxVertices[] = {

		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};


	//use openGL functionality to generate & bind data into buffers
	//Generating vertex Array with skyboxVAO and Buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	//binding the VertexArray andBuffers
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void MainGame::gameLoop()
{
	//While game state is not exit, process input and draw game
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

		}
	}
	
}

//Function used to pass variables into the reflection shader
void MainGame::setUpShaderReflection()
{
	shaderReflection.setMat4("model", transform.GetModel());
	shaderReflection.setMat4("view", myCamera.GetView());
	shaderReflection.setMat4("projection", myCamera.GetProjection());
	shaderReflection.setVec3("cameraPos", myCamera.getPos());
	shaderSkybox.setInt("skybox", 0);
}

//Function used to pass variables into the Explosion shader
void MainGame::setUpShaderExplosion()
{
	shaderExplosion.setMat4("transform", transform.GetModel());
	shaderExplosion.setFloat("time", counter);
}

//Function used to pass variables into the aditional shader
void MainGame::setUpShaderExtension()
{
	//Storing the light position
	glm::vec3 lightPosition = glm::vec3(-5, -2, -3);

	shaderExtension.setMat4("transform", transform.GetModel());

	shaderExtension.setVec3("toonLightPosition", lightPosition);
	shaderExtension.setVec3("bpLightPosition", lightPosition);
	shaderExtension.setVec3("viewPosition", myCamera.getPos());
}

//Function used to update the Skybox
void MainGame::SkyBox()
{
	glDepthFunc(GL_LEQUAL);
	//Binding the skybox
	shaderSkybox.Bind();

	//Passing variables into shader
	shaderSkybox.setInt("skybox", 0);
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetViewProjection());

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	SkyBox();

	//Updating shader, mesh, transform, related to enviroment shader

	transform.SetPos(glm::vec3(-2, 0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shaderReflection.Bind();
	setUpShaderReflection();
	shaderReflection.Update(transform, myCamera);
	texture.Bind(0);
	meshEnviroment.draw();


	//Updating shader, mesh, transform, related to explosion shader

	transform.SetPos(glm::vec3(2,0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	shaderExplosion.Bind();
	setUpShaderExplosion();
	shaderExplosion.Update(transform, myCamera);
	textureExplosion.Bind(0);
	meshExplosion.draw();


	//Updating shader, mesh, transform, related to extension shader

	transform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(5, 5, 5));

	shaderExtension.Bind();
	setUpShaderExtension();
	shaderExtension.Update(transform, myCamera);
	texture.Bind(0);
	meshExtension.draw();

	//Updating counter
	counter = counter + 0.01f;
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();
	_gameDisplay.swapBuffer();
} 