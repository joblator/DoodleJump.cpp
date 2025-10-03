#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>


struct GameData
{
	glm::vec2 rectPos = {100,100};
	int acceleration = 0;

}gameData;

gl2d::Renderer2D renderer;
gl2d::Texture block;
gl2d::Texture doodle;
gl2d::Font f;
const int maxAcceleration = 200;

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	//loading the saved data. Loading an entire structure like this makes savind game data very easy.
	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));


	block.loadFromFile(RESOURCES_PATH "test.jpg", true);
	doodle.loadFromFile(RESOURCES_PATH "Doodle.jpg", true);
	f.createFromFile(RESOURCES_PATH "roboto_black.ttf");

	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX (); //window width
	h = platform::getFrameBufferSizeY(); //window height
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);

#pragma endregion

#pragma region move stuff
	if (gameData.acceleration > maxAcceleration) {
		gameData.acceleration = maxAcceleration;
	}else if (gameData.acceleration < -maxAcceleration) {
		gameData.acceleration = -maxAcceleration;
	}else {
		gameData.acceleration += 2.5;
	}
	gameData.rectPos.y += gameData.acceleration * deltaTime;
	if (platform::isKeyHeld(platform::Button::A))
	{
		gameData.rectPos.x -= deltaTime * 200;
	}
	if (platform::isKeyHeld(platform::Button::D))
	{
		gameData.rectPos.x += deltaTime * 200;
	}
	if (platform::isKeyHeld(platform::Button::W) && gameData.acceleration > -maxAcceleration)
	{
		gameData.acceleration -= 50;
		gameData.rectPos.y += gameData.acceleration * deltaTime;
	}
	if (gameData.rectPos.y > 300 - 50 &&   250<= gameData.rectPos.x && gameData.rectPos.x <= 350)
	{	
		gameData.rectPos.y = 250;
		gameData.acceleration = -maxAcceleration;
	}
	gameData.rectPos = glm::clamp(gameData.rectPos, glm::vec2{0,0}, glm::vec2{w-50,h-50});
	renderer.renderRectangle({ glm::vec2{300,300},50,50 }, block);
	renderer.renderRectangle({ gameData.rectPos, 50, 50 }, doodle);
	// first line of added code!!!!!!
	ImGui::Begin("acceleration");
	ImGui::Text("acceleration: %d", int(gameData.acceleration));
	ImGui::Text("X pos: %d", int(gameData.rectPos.x));
	ImGui::Text("y Pos: %d", int(gameData.rectPos.y));
	ImGui::End();


	renderer.flush();



	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{

	//saved the data.
	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
