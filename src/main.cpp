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


struct Character{
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	long int advance;
};
std::map<GLchar, Character> characters; 


int SCR_WIDTH= 1280;
int SCR_HEIGHT = 720;

static bool isOpen = true;


GLuint VAO, VBO;

void renderText(std::string shadername, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color){
	glm::mat4 textprojection = glm::ortho(0.0f,static_cast<GLfloat>(SCR_WIDTH), 0.0f,static_cast<GLfloat>(SCR_HEIGHT), -100.0f,100.0f);
	ResourceManager::getShader(shadername).use();	
	ResourceManager::getShader(shadername).setVec3("textColor", color);
	ResourceManager::getShader(shadername).setMat4("projection", textprojection);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for(c = text.begin();c!=text.end();c++){
		Character ch = characters[*c];
		GLfloat xpos = x + ch.bearing.x *scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
		GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }       
		};
		glBindTexture(GL_TEXTURE_2D, ch.textureId);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale; 
	}
	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


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
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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

	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


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
	Model m("res/Models/hockeypuck/10511_Hockey_puck_v1_L3.obj");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	
	FT_Library ft;
	if(FT_Init_FreeType(&ft)){
		std::cout << "Could not init free type" << std::endl;
	}
	FT_Face face;
	if(FT_New_Face(ft, "res/fonts/digital-dream/DigitalDream.ttf", 0, &face)){
		std::cout << "failed to load a font" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(GLubyte c  = 0;c<128;c++){
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
			std::cout << "Failed to load a glyph" << std::endl;
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture, 
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        	glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


	ResourceManager::loadShader("text", "res/Shaders/text/text.vs" , "res/Shaders/text/text.fs");

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

		if (show_demo_window)
		    ImGui::ShowDemoWindow(&show_demo_window);

		static float rotateX; 
		{
		    ImGui::Begin("Hello, world!");                          

		    ImGui::Text("This is some useful text.");               
		    ImGui::Checkbox("Demo Window", &show_demo_window);      
		    ImGui::Checkbox("Another Window", &show_another_window);

		    ImGui::SliderFloat("RotateX", &rotateX, 0.0f, 360.0f);            
		    ImGui::ColorEdit3("clear color", (float*)&clear_color); 


		    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		    ImGui::End();
		}

		if (show_another_window)
		{
		    ImGui::Begin("Another Window", &show_another_window);   
		    ImGui::Text("Hello from another window!");
		    if (ImGui::Button("Close Me"))
			show_another_window = false;
		    ImGui::End();
		}

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
		model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1.0f,0.0f,0.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,6.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = projection *  view * model;


		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderText("text", "dobre", 640.0f, 360.0f, 1.0f,glm::vec3(1.0f,1.0f,1.0f));
		renderText("text", "POMARAŃCZOWE!", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		ResourceManager::getShader("sample").setMat4("mvp", mvp);
		ResourceManager::getShader("sample").use();
		m.draw("sample");
		
		game.state->update();
		game.state->handleInput();
		game.state->draw();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);

		startTime = endTime;
		endTime = SDL_GetTicks();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
		
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
