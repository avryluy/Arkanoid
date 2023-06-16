#pragma once

#include "SDL.h"
#include "Engine\FPSManager.h"
#include "Data\ScoreScreen.h"
#include "Data\Platform.h"
#include "../Ball.h"
#include "Engine\TextTextures.h"
//#include "GameConstants.h"
#include "Engine\Renderer.h"

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
	SDL_Surface* main_surf = nullptr;
	const char* TITLE = "Arkanoid - By Avry Luy";
	int countedframes = 0;
	ScoreScreen* scorescreen;
	Platform* platform;
	Ball* ball;
	TextTextures* text;
	TextTextures* subtext;
};

