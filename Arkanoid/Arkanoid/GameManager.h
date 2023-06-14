#pragma once

#include "SDL.h"
#include "GameConstants.h"
#include "Renderer.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	int Init();
	void GameLoop();
	void HandleEvents();
	void Render(const TSharedPtr<renderer>& nRenderer);
	void Quit();
private:
	bool gameRunning = false;
	SDL_Window* window = nullptr;
	TSharedPtr<class renderer> Renderer = nullptr;

};

