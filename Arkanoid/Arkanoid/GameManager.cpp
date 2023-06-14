#include "GameManager.h"


GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	Renderer = NULL;
}

int GameManager::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow("Arkanoid - By Avry Luy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	};
	//Create Renderer
	Renderer = renderer::Construct(window);
	//renderer::Construct(window);
	//Start gameLoop
	GameLoop();
	return 0;
}

void GameManager::GameLoop()
{

	gameRunning = true;
	while (gameRunning)
	{
		//HandleEvents();
		Render(Renderer);

	};
	
	//Quit();
}

void GameManager::HandleEvents()
{

}

void GameManager::Render(const TSharedPtr<renderer>& nRenderer)
{
	nRenderer->clear(88, 154, 186, 255);
	nRenderer->Present();
}

void GameManager::Quit()
{
	gameRunning = false;
	//renderer = NULL;
	SDL_Quit();
}