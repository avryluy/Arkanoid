#include "ScoreScreen.h"

ScoreScreen::ScoreScreen()
{
	this->x = (1080 / 1.5);
	this->y = 0;
	this->w = (1080 - this->x);
	this->h = 720;

	this->r = 255;
	this->g = 20;
	this->b = 0;
	this->a = 255;
}

ScoreScreen::~ScoreScreen()
{
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