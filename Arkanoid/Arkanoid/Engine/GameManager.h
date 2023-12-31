#pragma once

#include <SDL.h>
#include <vector>
#include <list>
#include "Data\AudioManager.h"
#include "Data\ScoreScreen.h"
#include "Data\Platform.h"
#include "Data\Ball.h"
#include "Data\Block.h"
#include "Engine\TextTextures.h"
#include "Engine\Templates.h"
#include "Engine\FPSManager.h"
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
	void Init_Level();
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
	Block *blocks;
	AudioManager* audioman;
	//std::list<Block*> targets;
	std::vector<Block> targets;
	TextTextures* text;
	TextTextures* subtext;
	TextTextures* life_count;
};

