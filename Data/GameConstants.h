#pragma once
#include <lib\x64\SDL.h>
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;
const float TICKS_PER_FRAME = 1000.f / FPS;

const int NUMBLOCKTYPES = 3;
const SDL_Color blockColor[NUMBLOCKTYPES] = {
	{255,0,0,255},
	{0,255,0,255},
	{0,0,255,255}
};

const int blockHealth[NUMBLOCKTYPES] = { 2, 3, 4 };

const int blockScores[NUMBLOCKTYPES] = { 700, 325, 175};



