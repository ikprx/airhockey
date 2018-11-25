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
public:
	virtual void handleInput();
	virtual void update();
	virtual void draw();
	Playstate(Game * game);
};
