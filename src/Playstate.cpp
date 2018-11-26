#include "Playstate.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "Physics.h"
#include <SDL2/SDL.h>
void Playstate::handleInput(SDL_Event event)
{
	Model * m = ResourceManager::getModel("player1");
	if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_w){
		if(m->acceleration != glm::vec2(0.0f,0.0f))
		{
			m->acceleration = glm::vec2(0.0f,0.0f);
		}
	}
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w){
		if(m->acceleration.y != 1.0f){
			m->acceleration.y = std::min( std::max(m->acceleration.y + 5.0f * game->dt, 1.0f), 1.0f);
		}	
	}
	if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_s){
		if(m->acceleration != glm::vec2(0.0f,0.0f))
		{
			m->acceleration = glm::vec2(0.0f,0.0f);
		}
	}
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s){
		if(m->acceleration.y != 1.0f){
			m->acceleration.y = std::min( std::max(m->acceleration.y - (5.0f * game->dt), -1.0f), -1.0f);
		}	
	}
}
void Playstate::update()
{
	if(score1 != 5 && score2 !=5){
		if(Physics::collisionDetection(ResourceManager::getModel("player1"), ResourceManager::getModel("puck")))
		{
			if(ResourceManager::getModel("puck")->transform.position.y > ResourceManager::getModel("player1")->transform.position.y)
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				ResourceManager::getModel("puck")->acceleration.y = 1.0f;
			}
			else if(ResourceManager::getModel("puck")->transform.position.y < ResourceManager::getModel("player1")->transform.position.y)
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				ResourceManager::getModel("puck")->acceleration.y = -1.0f;
			}
			else
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
			}
			lastCollision = "player1";
		}
		if(Physics::collisionDetection(ResourceManager::getModel("player2"), ResourceManager::getModel("puck")))
		{
			if(ResourceManager::getModel("puck")->transform.position.y > ResourceManager::getModel("player2")->transform.position.y)
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				ResourceManager::getModel("puck")->acceleration.y = 1.0f;
			}
			else if(ResourceManager::getModel("puck")->transform.position.y < ResourceManager::getModel("player2")->transform.position.y)
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				ResourceManager::getModel("puck")->acceleration.y = -1.0f;
			}
			else			
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
			}
			lastCollision = "player2";
		}
		if(ResourceManager::getModel("puck")->transform.position.x > ResourceManager::getModel("goal2")->transform.position.x + (ResourceManager::getModel("goal2")->boundariesMAX.x/2)* ResourceManager::getModel("goal2")->transform.scale.x){
			score1++;
			ResourceManager::getModel("puck")->transform.position = glm::vec3(0.0f,0.0f,0.0f);
			ResourceManager::getModel("puck")->acceleration = glm::vec2(3.0f,0.0f);
		}
		if(ResourceManager::getModel("puck")->transform.position.x < ResourceManager::getModel("goal")->transform.position.x - (ResourceManager::getModel("goal")->boundariesMAX.x/2)* ResourceManager::getModel("goal")->transform.scale.x){
			score2++;
			ResourceManager::getModel("puck")->transform.position = glm::vec3(0.0f,0.0f,0.0f);
			ResourceManager::getModel("puck")->acceleration = glm::vec2(-3.0f,0.0f);
		}
		if(Physics::collisionDetection(ResourceManager::getModel("puck"), ResourceManager::getModel("UPobstacle")))
		{
			if(lastCollision != "UPobstacle"){
				ResourceManager::getModel("puck")->acceleration.y *= -1.0f;
				lastCollision = "UPobstacle";
			}
		}
		if(Physics::collisionDetection(ResourceManager::getModel("puck"), ResourceManager::getModel("BOTTOMobstacle")))
		{
			if(lastCollision != "BOTTOMobstacle"){
				ResourceManager::getModel("puck")->acceleration.y *= -1.0f;
				lastCollision = "BOTTOMobstacle";
			}
		}
		if(Physics::collisionDetection(ResourceManager::getModel("puck"), ResourceManager::getModel("LEFTUPobstacle")))
		{
			if(lastCollision != "LEFTUPobstacle")
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				lastCollision = "LEFTUPobstacle";
			}
		}
		if(Physics::collisionDetection(ResourceManager::getModel("puck"), ResourceManager::getModel("LEFTBOTTOMobstacle")))
		{
			if(lastCollision != "LEFTBOTTOMobstacle")
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				lastCollision = "LEFTBOTTOMobstacle";
			}
		}
		if(Physics::collisionDetection(ResourceManager::getModel("puck"), ResourceManager::getModel("RIGHTUPobstacle")))
		{
			if(lastCollision != "RIGHTUPobstacle")
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				lastCollision = "RIGHTUPobstacle";
			}
		}
		if(Physics::collisionDetection(ResourceManager::getModel("puck"), ResourceManager::getModel("RIGHTBOTTOMobstacle")))
		{
			if(lastCollision != "RIGHTBOTTOMobstacle")
			{
				ResourceManager::getModel("puck")->acceleration.x *= -1.0f;
				lastCollision = "RIGHTBOTTOMobstacle";
			}
		}
		ResourceManager::getModel("puck")->transform.position.x += game->dt * ResourceManager::getModel("puck")->acceleration.x;
		ResourceManager::getModel("puck")->transform.position.y += game->dt * ResourceManager::getModel("puck")->acceleration.y;
		ResourceManager::getModel("player1")->transform.position.y += game->dt * ResourceManager::getModel("player1")->acceleration.y;
	}
	else{
		inputState = false;
		std::string output = "GRACZ ";
		output+= (score1 > score2 ? "1 " : "2 ");
		output+= "WYGRAL!";
		ResourceManager::text.renderText("text", output  , game->screenWidth/2-90,game->screenHeight/2, 1.0f, glm::vec3(1.0f,1.0f,1.0f));
	}
}
void Playstate::draw()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)game->screenWidth / (float)game->screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.getWorldToViewMatrix();

	ResourceManager::text.renderText("text", std::to_string(score1) + " : " + std::to_string(score2) , (game->screenWidth/2)-50,(game->screenHeight)-60, 1.0f, glm::vec3(1.0f,0.0f,0.0f));
	ResourceManager::getModel("player1")->draw("sample", view, projection);
	ResourceManager::getModel("player2")->draw("sample", view, projection);
	ResourceManager::getModel("puck")->draw("sample", view, projection);
	/*
	ResourceManager::getModel("UPobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("BOTTOMobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("LEFTUPobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("LEFTBOTTOMobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("RIGHTUPobstacle")->draw("sample", view, projection);
	ResourceManager::getModel("RIGHTBOTTOMobstacle")->draw("sample", view, projection);
	*/
	ResourceManager::getModel("field")->draw("sample", view, projection);
	ResourceManager::getModel("goal")->draw("sample", view, projection);
	ResourceManager::getModel("goal2")->draw("sample", view, projection);
}

Playstate::Playstate(Game * game){
	this->game = game;
	score1 = 0;
	score2 = 0;

	ResourceManager::loadShader("sample", "res/Shaders/test/sample.vs", "res/Shaders/test/sample.fs");
	ResourceManager::loadModel("player1", "res/Models/player1/player.obj");
	ResourceManager::loadModel("player2", "res/Models/player2/player.obj");
	ResourceManager::loadModel("puck", "res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj");
	ResourceManager::loadModel("field", "res/Models/field/field.obj");
	ResourceManager::loadModel("goal", "res/Models/goal/Hockey Gates model.obj");
	ResourceManager::getModel("goal")->color = glm::vec3(1.0f,0.0f,0.0f);
	ResourceManager::loadModel("goal2", "res/Models/goal/Hockey Gates model.obj");
	ResourceManager::getModel("goal2")->color = glm::vec3(1.0f,0.0f,0.0f);
	ResourceManager::loadModel("UPobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("BOTTOMobstacle", "res/Models/obstacle/obstacle.obj");
	ResourceManager::loadModel("LEFTUPobstacle", "res/Models/obstacleY/obstacle.obj");
	ResourceManager::loadModel("LEFTBOTTOMobstacle", "res/Models/obstacleY/obstacle.obj");
	ResourceManager::loadModel("RIGHTUPobstacle", "res/Models/obstacleY/obstacle.obj");
	ResourceManager::loadModel("RIGHTBOTTOMobstacle", "res/Models/obstacleY/obstacle.obj");
	ResourceManager::loadShader("text", "res/Shaders/text/text.vs" , "res/Shaders/text/text.fs");
	ResourceManager::text.setup(game->screenWidth, game->screenHeight, 48, "res/fonts/digital-dream/DigitalDream.ttf");

	ResourceManager::getModel("player1")->transform.scale = glm::vec3(0.25f,0.25f,0.25f);
	ResourceManager::getModel("player2")->transform.scale = glm::vec3(0.25f,0.25f,0.25f);

	ResourceManager::getModel("puck")->transform.scale = glm::vec3(0.10f,0.1f,0.1f);
	ResourceManager::getModel("puck")->acceleration = glm::vec2(3.0f,0.0f);
	ResourceManager::getModel("player1")->transform.position = glm::vec3(-5.0f,0.0f,0.0f);
	ResourceManager::getModel("player2")->transform.position = glm::vec3(5.0f,0.0f,0.0f);

	ResourceManager::getModel("UPobstacle")->transform.position = glm::vec3(0.0f,4.0f,0.0f);
	ResourceManager::getModel("UPobstacle")->transform.scale = glm::vec3(1.7f,1.0f,1.0f);

	ResourceManager::getModel("BOTTOMobstacle")->transform.position = glm::vec3(0.0f,-4.0f,0.0f);
	ResourceManager::getModel("BOTTOMobstacle")->transform.scale = glm::vec3(1.7f,1.0f,1.0f);

	ResourceManager::getModel("LEFTUPobstacle")->transform.position = glm::vec3(-5.225f,4.0f,0.0f);
	ResourceManager::getModel("LEFTUPobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);

	ResourceManager::getModel("LEFTBOTTOMobstacle")->transform.position = glm::vec3(-5.225f,-4.0f,0.0f);
	ResourceManager::getModel("LEFTBOTTOMobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);

	ResourceManager::getModel("RIGHTUPobstacle")->transform.position = glm::vec3(5.225f,4.0f,0.0f);
	ResourceManager::getModel("RIGHTUPobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);

	ResourceManager::getModel("RIGHTBOTTOMobstacle")->transform.position = glm::vec3(5.225f,-4.0f,0.0f);
	ResourceManager::getModel("RIGHTBOTTOMobstacle")->transform.scale = glm::vec3(1.0f,0.463f, 1.0f);
	
	ResourceManager::getModel("field")->transform.rotation.x = 90.0f;
	this->camera.position = glm::vec3(0.0f,0.0f,10.0f);


	ResourceManager::getModel("goal")->transform.scale = glm::vec3(0.001f,0.0025f,0.001f);
	ResourceManager::getModel("goal")->transform.position = glm::vec3(-5.1f,0.0f,0.0f);
	ResourceManager::getModel("goal")->transform.rotation = glm::vec3(90.0f,90.0f,0.0f);
	
	ResourceManager::getModel("goal2")->transform.scale = glm::vec3(0.001f,0.0025f,0.001f);
	ResourceManager::getModel("goal2")->transform.position = glm::vec3(5.1f,0.0f,0.0f);
	ResourceManager::getModel("goal2")->transform.rotation = glm::vec3(90.0f,270.0f,0.0f);
}
