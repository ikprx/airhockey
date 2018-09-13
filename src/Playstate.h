#pragma once
#include "Game.h"
#include <iostream>
class Game;

class Playstate: public GameState{
public:
	virtual void handleInput();
	virtual void update();
	virtual void draw();
	Playstate(Game * game);
};
