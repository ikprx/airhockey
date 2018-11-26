#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ResourceManager.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Camera.h"
#include "Transform.h"
#define TEST_USE_IMGUI


int SCR_WIDTH= 1920;
int SCR_HEIGHT = 1080;
static bool isOpen = true;

bool cameraWindow = false;
bool positionWindow = false;
bool positionWindow2 = false;
bool positionWIndow3 = false;


int main(int argc, char * argv[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

	
	SDL_Window * window = SDL_CreateWindow("Air Hockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH,SCR_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
    	glewExperimental = GL_TRUE;	
	
	glewInit();

#ifdef TEST_USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();
#endif

	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 60;
	short timePerFrame = 16;


	SDL_Event event;

	Game game(SCR_WIDTH, SCR_HEIGHT);
	game.init();


	glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


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
		game.dt = (float)delta/1000;

		while(SDL_PollEvent(&event)){
			game.state->handleInput(event);
			#ifdef TEST_USE_IMGUI
			ImGui_ImplSDL2_ProcessEvent(&event);
			#endif
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:
							isOpen = false;
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
#ifdef TEST_USE_IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		{
		    ImGui::Begin("");                          

		    ImGui::Checkbox("Camera Window", &cameraWindow);
		    ImGui::Checkbox("Position Window", &positionWindow);
		    ImGui::Checkbox("Position Window2", &positionWindow2);
		    ImGui::Checkbox("Position Window3", &positionWIndow3);
		    ImGui::End();
		}

		if (cameraWindow)
		{
		    ImGui::Begin("Camera Window", &cameraWindow);   
		    ImGui::SliderFloat("pos->x", &game.state->camera.position.x, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->y", &game.state->camera.position.y, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->z", &game.state->camera.position.z, -100.0f, 100.0f);
		    
		    if (ImGui::Button("Close Me"))
			cameraWindow = false;
		    ImGui::End();
		}

		if(positionWindow){
		    ImGui::Begin("position Window", &positionWindow);   
		    ImGui::SliderFloat("pos->x", &ResourceManager::getModel("player1")->transform.position.x, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->y", &ResourceManager::getModel("player1")->transform.position.y, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->z", &ResourceManager::getModel("player1")->transform.position.z, -10.0f, 10.0f);
		    ImGui::SliderFloat("rot->x", &ResourceManager::getModel("player1")->transform.rotation.x, 0, 180.0f);
		    ImGui::SliderFloat("rot->y", &ResourceManager::getModel("player1")->transform.rotation.y, 0, 180);
		    ImGui::SliderFloat("rot->z", &ResourceManager::getModel("player1")->transform.rotation.z, 0, 180);
		    
		    if (ImGui::Button("Close Me"))
				positionWindow = false;
		    ImGui::End();

		}

		if(positionWindow2){
		    ImGui::Begin("position Window2", &positionWindow);   
		    ImGui::SliderFloat("pos->x", &ResourceManager::getModel("player2")->transform.position.x, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->y", &ResourceManager::getModel("player2")->transform.position.y, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->z", &ResourceManager::getModel("player2")->transform.position.z, -10.0f, 10.0f);
		    ImGui::SliderFloat("rot->x", &ResourceManager::getModel("player2")->transform.rotation.x, 0, 180.0f);
		    ImGui::SliderFloat("rot->y", &ResourceManager::getModel("player2")->transform.rotation.y, 0, 180);
		    ImGui::SliderFloat("rot->z", &ResourceManager::getModel("player2")->transform.rotation.z, 0, 180);
		    
		    if (ImGui::Button("Close Me"))
				positionWindow2 = false;
		    ImGui::End();
		}

                if(positionWIndow3){
                    ImGui::Begin("position Window3", &positionWindow);   
                    ImGui::SliderFloat("pos->x", &ResourceManager::getModel("puck")->transform.position.x, -10.0f, 10.0f);
                    ImGui::SliderFloat("pos->y", &ResourceManager::getModel("puck")->transform.position.y, -10.0f, 10.0f);
                    ImGui::SliderFloat("pos->z", &ResourceManager::getModel("puck")->transform.position.z, -10.0f, 10.0f);
                    ImGui::SliderFloat("rot->x", &ResourceManager::getModel("puck")->transform.rotation.x, 0, 180.0f);
                    ImGui::SliderFloat("rot->y", &ResourceManager::getModel("puck")->transform.rotation.y, 0, 180);
                    ImGui::SliderFloat("rot->z", &ResourceManager::getModel("puck")->transform.rotation.z, 0, 180);
                    ImGui::SliderFloat("scale->x", &ResourceManager::getModel("puck")->transform.scale.x, 0, 5.0f);
                    ImGui::SliderFloat("scale->y", &ResourceManager::getModel("puck")->transform.scale.y, 0, 5.0f);
                    ImGui::SliderFloat("scale->z", &ResourceManager::getModel("puck")->transform.scale.z, 0, 5.0f);
                    if (ImGui::Button("Close Me"))
                                positionWindow2 = false;
                    ImGui::End();

                }
		ImGui::Render();
#endif
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game.state->update();
		game.state->draw();
#ifdef TEST_USE_IMGUI
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
		SDL_GL_SwapWindow(window);
		startTime = endTime;
		endTime = SDL_GetTicks();
	}
#ifdef TEST_USE_IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif
		
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
