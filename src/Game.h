#pragma once
#include <GL/glew.h>
#include "Camera.h"
#include <SDL2/SDL.h>
class Game;

class GameState{
public:
	Camera camera;
	virtual void handleInput(SDL_Event event) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	Game * game;
};

class Game{
public:
	int screenWidth, screenHeight;
	float dt;	

	GameState * state;
	Game(int screenWidth, int screenHeight);
	~Game();

	void init();
};
