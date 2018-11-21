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
	if(Physics::collisionDetection(ResourceManager::getModel("gracz1"), ResourceManager::getModel("test")))
	{
		std::cout << "Collission occured! Ticks: " << SDL_GetTicks() << std::endl;
	}
	ResourceManager::getModel("gracz1")->transform.position.x += game->dt;
}
void Playstate::draw()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)game->screenWidth / (float)game->screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.getWorldToViewMatrix();
	ResourceManager::text.renderText("text", "Test", 0.0f,20.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f));

	ResourceManager::getModel("gracz1")->draw("sample", view, projection);
	ResourceManager::getModel("test")->draw("sample", view, projection);
}

Playstate::Playstate(Game * game){
	this->game = game;

	ResourceManager::loadShader("sample", "res/Shaders/test/sample.vs", "res/Shaders/test/sample.fs");
	ResourceManager::loadModel("gracz1", "res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj");
	ResourceManager::loadModel("test", "res/Models/test/test.obj");
	ResourceManager::loadShader("text", "res/Shaders/text/text.vs" , "res/Shaders/text/text.fs");
	ResourceManager::text.setup(game->screenWidth, game->screenHeight, 24, "res/fonts/digital-dream/DigitalDream.ttf");


	this->camera.position = glm::vec3(0.0f,0.0f,12.0f);
}
