#include "Data\ScoreScreen.h"

ScoreScreen::ScoreScreen()
{
	this->x = (int)(1080 / 1.5);
	this->y = 0;
	this->w = (1080 - this->x);
	this->h = this->x;

	this->r = 50;
	this->g = 120;
	this->b = 186;
	this->a = 255;
	//SDL_Log("ScoreScreen()\n");
}

ScoreScreen::~ScoreScreen()
{
	//SDL_Log("~ScoreScreen()\n");
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

void ScoreScreen::render(const TSharedPtr<renderer>& nRenderer, int x, int y, int txtrWidth, int txtrHeight, SDL_Texture* mTexture, SDL_Rect* clip)
{
	SDL_Rect renderquad = { this->x, this->y, txtrWidth, txtrHeight };

	//if (clip != NULL)
	//{
	//	clip->w = txtrWidth;
	//	clip->h = txtrHeight;
	//}

	renderquad.w = this->w;
	renderquad.h = this->h;

	SDL_RenderCopy(nRenderer->GetNativeRenderer(), mTexture, clip, &renderquad);
}

int ScoreScreen::get_x()
{
	return this->x;
}

int ScoreScreen::get_y()
{
	return this->y;
}

int ScoreScreen::get_w()
{
	return this->w;
}

int ScoreScreen::get_h()
{
	return this->h;
}