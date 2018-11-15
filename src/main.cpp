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



int SCR_WIDTH= 1280;
int SCR_HEIGHT = 720;

static bool isOpen = true;

bool detectionCircleAABB(std::string modelName1, std::string modelName2){
	Model * m1 = ResourceManager::getModel(modelName1);
	Model * m2 = ResourceManager::getModel(modelName2);
	bool collisionXMIN = (m1->transform.position.x + m1->boundariesMAX.x) >= m2->boundariesMIN.x  + m2->transform.position.x;
	bool collisionXMAX = (m1->transform.position.x + m1->boundariesMIN.x) <= m2->boundariesMAX.x  + m2->transform.position.x;
	
	bool collisionYMIN = (m1->transform.position.y + m1->boundariesMAX.y) >= (m2->transform.position.y + m2->boundariesMIN.y);
	bool collisionYMAX = (m1->transform.position.y + m1->boundariesMIN.y) <= m2->boundariesMAX.y  + m2->transform.position.y;
	
	//bool collisionY = m1->transform.position.y + m1->boundariesMAX.y >= m2->boundariesMAX.y + m2->transform.position.y;
	return collisionXMIN && collisionXMAX && collisionYMAX && collisionYMIN;
}

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

	bool cameraWindow = false;
	bool positionWindow = false;
	bool positionWindow2 = false;

	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 60;
	short timePerFrame = 16;


	SDL_Event event;

	Game game;
	game.init();

	ResourceManager::loadShader("sample", "res/Shaders/test/sample.vs", "res/Shaders/test/sample.fs");
	ResourceManager::loadModel("gracz1", "res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj");
	ResourceManager::loadModel("test", "res/Models/test/test.obj");

	glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.1f,0.3f,0.4f,1.0f);

	ResourceManager::loadShader("text", "res/Shaders/text/text.vs" , "res/Shaders/text/text.fs");
	ResourceManager::text.setup(SCR_WIDTH, SCR_HEIGHT, 24, "res/fonts/digital-dream/DigitalDream.ttf");
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

		    ImGui::Checkbox("Camera Window", &cameraWindow);
		    ImGui::Checkbox("Position Window", &positionWindow);
		    ImGui::Checkbox("Position Window2", &positionWindow2);
		    ImGui::End();
		}

		if (cameraWindow)
		{
		    ImGui::Begin("Camera Window", &cameraWindow);   
		    ImGui::SliderFloat("pos->x", &camera.position.x, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->y", &camera.position.y, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->z", &camera.position.z, -10.0f, 10.0f);
		    
		    if (ImGui::Button("Close Me"))
			cameraWindow = false;
		    ImGui::End();
		}

		if(positionWindow){
		    ImGui::Begin("position Window", &positionWindow);   
		    ImGui::SliderFloat("pos->x", &ResourceManager::getModel("gracz1")->transform.position.x, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->y", &ResourceManager::getModel("gracz1")->transform.position.y, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->z", &ResourceManager::getModel("gracz1")->transform.position.z, -10.0f, 10.0f);
		    ImGui::SliderFloat("rot->x", &ResourceManager::getModel("gracz1")->transform.rotation.x, 0, 180.0f);
		    ImGui::SliderFloat("rot->y", &ResourceManager::getModel("gracz1")->transform.rotation.y, 0, 180);
		    ImGui::SliderFloat("rot->z", &ResourceManager::getModel("gracz1")->transform.rotation.z, 0, 180);
		    
		    if (ImGui::Button("Close Me"))
				positionWindow = false;
		    ImGui::End();

		}

		if(positionWindow2){
		    ImGui::Begin("position Window2", &positionWindow);   
		    ImGui::SliderFloat("pos->x", &ResourceManager::getModel("test")->transform.position.x, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->y", &ResourceManager::getModel("test")->transform.position.y, -10.0f, 10.0f);
		    ImGui::SliderFloat("pos->z", &ResourceManager::getModel("test")->transform.position.z, -10.0f, 10.0f);
		    ImGui::SliderFloat("rot->x", &ResourceManager::getModel("test")->transform.rotation.x, 0, 180.0f);
		    ImGui::SliderFloat("rot->y", &ResourceManager::getModel("test")->transform.rotation.y, 0, 180);
		    ImGui::SliderFloat("rot->z", &ResourceManager::getModel("test")->transform.rotation.z, 0, 180);
		    
		    if (ImGui::Button("Close Me"))
				positionWindow2 = false;
		    ImGui::End();

		}

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getWorldToViewMatrix();


		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ResourceManager::text.renderText("text", "Test", 0.0f,20.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f));

		ResourceManager::getModel("gracz1")->draw("sample", view, projection);
		ResourceManager::getModel("test")->draw("sample", view, projection);
		

		game.state->update();
		game.state->handleInput();
		game.state->draw();
		
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);

		if(detectionCircleAABB("gracz1","test")){
			std::cout << delta << " COLLISION" << std::endl;
		}

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
