#pragma once
#include <GL/glew.h>

class Game;

class GameState{
public:
	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	Game * game;
};

class Game{
public:
	float dt =49.0f;
	
	GameState * state;
	Game();
	~Game();

	void init();
};
