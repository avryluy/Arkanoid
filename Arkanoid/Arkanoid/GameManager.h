#pragma once

#include "SDL.h"
#include "FPSManager.h"
#include "ScoreScreen.h"
#include "Platform.h"
#include "TextTextures.h"
//#include "GameConstants.h"
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
	SDL_Surface* main_surf = nullptr;
	const char* TITLE = "Arkanoid - By Avry Luy";
	int countedframes = 0;
	ScoreScreen* scorescreen;
	Platform* platform;
	TextTextures* text;
	TextTextures* subtext;
};

