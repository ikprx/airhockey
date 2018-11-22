#include "Playstate.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "Physics.h"
#include <SDL2/SDL.h>
void Playstate::handleInput()
{

}
void Playstate::update()
{
	if(Physics::collisionDetection(ResourceManager::getModel("player1"), ResourceManager::getModel("puck")))
	{
		ResourceManager::getModel("puck")->acceleration *= -1.0f;
	}
	if(Physics::collisionDetection(ResourceManager::getModel("player2"), ResourceManager::getModel("puck")))
	{
		ResourceManager::getModel("puck")->acceleration *= -1.0f;
	}
	ResourceManager::getModel("puck")->transform.position.x += game->dt * ResourceManager::getModel("puck")->acceleration;
}
void Playstate::draw()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)game->screenWidth / (float)game->screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.getWorldToViewMatrix();
	ResourceManager::text.renderText("text", "Test", 0.0f,20.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f));

	ResourceManager::getModel("player1")->draw("sample", view, projection);
	ResourceManager::getModel("player2")->draw("sample", view, projection);
	ResourceManager::getModel("puck")->draw("sample", view, projection);
	ResourceManager::getModel("UPobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("BOTTOMobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("LEFTUPobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("LEFTBOTTOMobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("RIGHTUPobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("RIGHTBOTTOMobstacle")->draw("sample", view, projection);
}

Playstate::Playstate(Game * game){
	this->game = game;

	ResourceManager::loadShader("sample", "res/Shaders/test/sample.vs", "res/Shaders/test/sample.fs");
	ResourceManager::loadModel("player1", "res/Models/player1/player.obj");
	ResourceManager::loadModel("player2", "res/Models/player2/player.obj");
	ResourceManager::loadModel("puck", "res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj");
	ResourceManager::loadModel("UPobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("BOTTOMobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("LEFTUPobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("LEFTBOTTOMobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("RIGHTUPobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("RIGHTBOTTOMobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadShader("text", "res/Shaders/text/text.vs" , "res/Shaders/text/text.fs");
	ResourceManager::text.setup(game->screenWidth, game->screenHeight, 24, "res/fonts/digital-dream/DigitalDream.ttf");

	ResourceManager::getModel("player1")->transform.scale = glm::vec3(0.25f,0.25f,0.25f);
	ResourceManager::getModel("player2")->transform.scale = glm::vec3(0.25f,0.25f,0.25f);

	ResourceManager::getModel("puck")->transform.scale = glm::vec3(0.10f,0.1f,0.1f);
	ResourceManager::getModel("puck")->acceleration = 2.0f;
	ResourceManager::getModel("player1")->transform.position = glm::vec3(-5.0f,0.0f,0.0f);
	ResourceManager::getModel("player2")->transform.position = glm::vec3(5.0f,0.0f,0.0f);

	ResourceManager::getModel("UPobstacle")->transform.rotation = glm::vec3(0.0f,0.0f,90.0f);
	ResourceManager::getModel("UPobstacle")->transform.position = glm::vec3(0.0f,4.0f,0.0f);
	ResourceManager::getModel("UPobstacle")->transform.scale = glm::vec3(1.7f,1.0f,1.0f);

	ResourceManager::getModel("BOTTOMobstacle")->transform.rotation = glm::vec3(0.0f,0.0f,90.0f);
	ResourceManager::getModel("BOTTOMobstacle")->transform.position = glm::vec3(0.0f,-4.0f,0.0f);
	ResourceManager::getModel("BOTTOMobstacle")->transform.scale = glm::vec3(1.7f,1.0f,1.0f);

	ResourceManager::getModel("LEFTUPobstacle")->transform.position = glm::vec3(-5.225f,2.5f,0.0f);
	ResourceManager::getModel("LEFTUPobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);
	ResourceManager::getModel("LEFTBOTTOMobstacle")->transform.position = glm::vec3(-5.225f,-2.5f,0.0f);
	ResourceManager::getModel("LEFTBOTTOMobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);
	ResourceManager::getModel("RIGHTUPobstacle")->transform.position = glm::vec3(5.225f,2.5f,0.0f);
	ResourceManager::getModel("RIGHTUPobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);
	ResourceManager::getModel("RIGHTBOTTOMobstacle")->transform.position = glm::vec3(5.225f,-2.5f,0.0f);
	ResourceManager::getModel("RIGHTBOTTOMobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);
	this->camera.position = glm::vec3(0.0f,0.0f,12.0f);
}
