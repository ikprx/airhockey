#pragma once
#include "Game.h"
#include <iostream>
#include "ResourceManager.h"
#include "Camera.h"

class Game;

class Playstate: public GameState{
	int score1, score2;
	bool inputState;
	std::string lastCollision;
	void controlOpponent();
	uint32_t nextControlTime;
public:
	virtual void handleInput(SDL_Event event);
	virtual void update();
	virtual void draw();
	Playstate(Game * game);
};
