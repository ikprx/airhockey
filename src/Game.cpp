#include "Game.h"
#include "Playstate.h"

Game::Game()
{

}
Game::~Game()
{

	
}
void Game::init()
{
	state = new Playstate(this);
}
