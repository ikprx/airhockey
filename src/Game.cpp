#include "Game.h"
#include "Playstate.h"

Game::Game(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}
Game::~Game()
{
	
}
void Game::init()
{
	state = new Playstate(this);
}
