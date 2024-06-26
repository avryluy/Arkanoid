#pragma once

#include <lib\x64\SDL.h>
#include <resource.h>
#include <vector>
#include <list>
#include "Data\AudioManager.h"
#include "Data\ScoreScreen.h"
#include "Data\Platform.h"
#include "Data\Ball.h"
#include "Data\Block.h"
#include "Engine/LTexture.h"
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
	int loadAssets();
	void GameLoop();
	void HandleEvents();
	void Render(const TSharedPtr<renderer>& nRenderer);
	void Init_Level();
	void Quit();

private:
	bool gameRunning = false;
	bool gameState;
	SDL_Window* window = nullptr;
	TSharedPtr<class renderer> Renderer = nullptr;
	SDL_Surface* main_surf = nullptr;
	const char* TITLE = "Arkanoid - By Avry Luy";
	int countedframes = 0;
	int gameScore = 0;
	ScoreScreen* scorescreen;
	Platform* platform;
	Ball* ball;
	Block *blocks;
	std::vector<Block> remainingBlocks;

	AudioManager audiomanager;
	const char* audiofile[7]; //music intro, block hit, brick destroy, ball death, life equal zero, level completed, collision hit
	soundData musIntro;
	soundData blockHitSnd;
	soundData colHitSnd;
	soundData blockDestroySnd;
	soundData ballDeathSnd;
	soundData gameOverSnd;
	soundData gameCompleteSnd;
	soundData sound1;
	soundData sound2;
	soundData sound3;
	
	std::vector<Block> targets;
	TextTextures* text;
	TextTextures* subtext;
	TextTextures* instructions;
	TextTextures* instructions2;
	TextTextures* life_count;
	TextTextures* score_count;
	TextTextures* game_over;
	TextTextures* game_win;
	const char* paddleBallFile;
	const char* bricksFile;
	LTexture* paddle_ball;
	SDL_Rect pballarray[2];
	LTexture* block_texture;
	SDL_Rect Tblock[12];
	LTexture* scoreScreenTexture;
	LTexture* mainScreenTexture;
	SDL_Rect mainScreenRect;
	SDL_Rect ScreenRects[2]; //Main window and score screen
	const char* backgroundFile;
	const char* instructionsText = "Left/Right Arrow to move | Space to Launch ball";
	const char* instructions2Text = "R to reset game | Esc to Quit";
};

