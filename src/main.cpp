#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ResourceManager.h"

const int SCR_WIDTH= 1280;
const int SCR_HEIGHT = 720;

static bool isOpen = true;
int main(int argc, char * argv[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
	
	SDL_Window * window = SDL_CreateWindow("Air Hockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH,SCR_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	glewInit();

	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 60;
	short timePerFrame = 16;


	SDL_Event event;
	glClearColor(0.1f,0.3f,0.4f,1.0f);

	Game game;
	game.init();

	ResourceManager::loadShader("sample", "res/Shaders/test/sample.vs", "res/Shaders/test/sample.fs");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Model m("res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj");
	//Model m("res/Models/test/test.obj");

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);


	glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
	float xRot = 0.0f;
	while(isOpen){
		if(!startTime){
			startTime = SDL_GetTicks();
		}
		else{
			delta = endTime - startTime;
		}

		if(delta<timePerFrame){
			SDL_Delay(timePerFrame-delta);
		}

		if(delta>timePerFrame){
			fps = 1000/delta;
		}
		game.dt = delta;

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_t:
							cameraPos.z += 1.0f;
							break;
						case SDLK_g:
							cameraPos.z -= 1.0f;
							break;
						case SDLK_u:
							if( xRot == 0){
							xRot = 90.0f;
							}
							else if(xRot == 90.0f){
								xRot = 180.0f;
							}
							else if(xRot == 180.0f){	
								xRot = 0.0f;
							}
							break;

						default:
							break;
					}
					break;
				case SDL_QUIT:
					isOpen = false;
					break;
				defualt:
					break;
			}
		}

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
		model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(xRot), glm::vec3(1.0f,0.0f,0.0f));
		glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = projection *  view * model;


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ResourceManager::getShader("sample").setMat4("mvp", mvp);
		ResourceManager::getShader("sample").use();
		m.draw("sample");
		
		game.state->update();
		game.state->handleInput();
		game.state->draw();
		SDL_GL_SwapWindow(window);

		startTime = endTime;
		endTime = SDL_GetTicks();
	}
		
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
