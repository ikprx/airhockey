#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

const int SCR_WIDTH = 512;
const int SCR_HEIGTH = 512;

static bool isOpen = true;
int main(int argc, char * argv[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_Window * window = SDL_CreateWindow("Air Hockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGTH, SDL_WINDOW_OPENGL);
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
	Model m("res/Models/test/test.obj");

	//glEnable(GL_DEPTH_TEST);
	glViewport(0,0,SCR_WIDTH, SCR_HEIGTH);
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
					if(event.key.keysym.sym == SDLK_ESCAPE){	
						isOpen = false;
					}
					break;
				case SDL_QUIT:
					isOpen = false;
					break;
				defualt:
					break;
			}
		}

		glm::mat4 projection = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,-100.0f,100.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-3.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
		view = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		glm::mat4 mvp = projection *  view * model;


		//ResourceManager::getShader("sample").setMat4("mvp", mvp);
		ResourceManager::getShader("sample").use();
		glClear(GL_COLOR_BUFFER_BIT);
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
