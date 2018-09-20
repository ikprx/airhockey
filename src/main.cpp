#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"

#include "ResourceManager.h"

#define SCR_WIDTH 512
#define SCR_HEIGTH 512

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
		

	float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


	ResourceManager::loadShader("sample", "res/sample.vs", "res/sample.fs");



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


		glClear(GL_COLOR_BUFFER_BIT);
		ResourceManager::getShader("sample").use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

		game.state->update();
		game.state->handleInput();
		game.state->draw();
		SDL_GL_SwapWindow(window);

		std::cout << "FPS is: " << fps << std::endl;
		startTime = endTime;
		endTime = SDL_GetTicks();
	}
		
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
