#include "GameManager.h"


GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

int GameManager::Init() {
	if (SDL_INIT_EVERYTHING == 0)
	{
		SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	};

	return 0;
}

void GameManager::GameLoop()
{

}