#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>


int main(int argc, char * argv[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_Window * window = SDL_CreateWindow("Ice Hockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewInit();

	std::cout << glGetString(GL_VERSION) << std::endl;

	SDL_Delay(4000);


}
