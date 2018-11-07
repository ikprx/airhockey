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
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#define IMGUI_IMPL_OPENGL_LOADER_GL3W 0 
#include <imgui_impl_opengl3.h>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Text.h"
#include "Camera.h"



int SCR_WIDTH= 1920;
int SCR_HEIGHT = 1080;

static bool isOpen = true;

int main(int argc, char * argv[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

	
	SDL_Window * window = SDL_CreateWindow("Air Hockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH,SCR_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
    	glewExperimental = GL_TRUE;	
	
	glewInit();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();

	bool show_another_window = false;
	
	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 60;
	short timePerFrame = 16;


	SDL_Event event;

	Game game;
	game.init();

	ResourceManager::loadShader("sample", "res/Shaders/test/sample.vs", "res/Shaders/test/sample.fs");
	Model m("res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj", "false");
	glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.1f,0.3f,0.4f,1.0f);

	ResourceManager::loadShader("text", "res/Shaders/text/text.vs" , "res/Shaders/text/text.fs");

	Text text;
	text.setup(SCR_WIDTH, SCR_HEIGHT, 48, "res/fonts/digital-dream/DigitalDream.ttf");

	Camera camera;
	camera.position = glm::vec3(0.0f,0.0f,12.0f);

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
			ImGui_ImplSDL2_ProcessEvent(&event);
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
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		{
		    ImGui::Begin("");                          

		    ImGui::Checkbox("Camera Window", &show_another_window);

		    ImGui::End();
		}

		if (show_another_window)
		{
		    ImGui::Begin("Camera Window", &show_another_window);   
		    ImGui::SliderFloat("pos->x", &camera.position.x, -100.0f, 100.0f);
		    ImGui::SliderFloat("pos->y", &camera.position.y, -100.0f, 100.0f);
		    ImGui::SliderFloat("pos->z", &camera.position.z, -100.0f, 100.0f);
		    
		    if (ImGui::Button("Close Me"))
			show_another_window = false;
		    ImGui::End();
		}

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
		model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f,.5f,0.2f));
		glm::mat4 view = camera.getWorldToViewMatrix();
		glm::mat4 mvp = projection *  view * model;


		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		text.renderText("text", "Sample Text", 0.0f,900.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f));
		ResourceManager::getShader("sample").use();
		ResourceManager::getShader("sample").setMat4("mvp", mvp);
		m.draw("sample");
		
		game.state->update();
		game.state->handleInput();
		game.state->draw();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);

		startTime = endTime;
		endTime = SDL_GetTicks();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

		
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
