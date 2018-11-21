#pragma once
#include "Game.h"
#include <iostream>
#include "ResourceManager.h"
#include "Camera.h"

class Game;

class Playstate: public GameState{
public:
	Camera camera;
	virtual void handleInput();
	virtual void update();
	virtual void draw();
	Playstate(Game * game);
};
