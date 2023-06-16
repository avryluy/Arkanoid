#include "Data\ScoreScreen.h"

ScoreScreen::ScoreScreen()
{
	this->x = (1080 / 1.5);
	this->y = 0;
	this->w = (1080 - this->x);
	this->h = this->x;

	this->r = 50;
	this->g = 120;
	this->b = 186;
	this->a = 255;
	SDL_Log("ScoreScreen()\n");
}

ScoreScreen::~ScoreScreen()
{
	SDL_Log("~ScoreScreen()\n");
}

void ScoreScreen::draw_screen(const TSharedPtr<renderer>& nRenderer)
{
	SDL_Rect mRect;
	mRect.x = x;
	mRect.y = y;
	mRect.w = w;
	mRect.h = h;
	nRenderer->DrawRect(mRect.x, mRect.y, mRect.w, mRect.h, this->r, this->g, this->b, this->a);

}

int ScoreScreen::get_x()
{
	return this->x;
}

int ScoreScreen::get_y()
{
	return this->y;
}