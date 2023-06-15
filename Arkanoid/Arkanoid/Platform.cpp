#include "Platform.h"


Platform::Platform()
{

	this->x = 100;
	this->y = 600;
	this->w = 200;
	this->h = 35;

	this->r = 255;
	this->g = 255;
	this->b = 45;
	this->a = 255;
	SDL_Log("Platform()\n");

}

Platform::~Platform()
{
	SDL_Log("~Platform()\n");

}



void Platform::draw_screen(const TSharedPtr<renderer>& nRenderer)
{
	SDL_Rect mRect;
	mRect.x = x;
	mRect.y = y;
	mRect.w = w;
	mRect.h = h;
	nRenderer->DrawRect(mRect.x, mRect.y, mRect.w, mRect.h, this->r, this->g, this->b, this->a);

}

int Platform::get_x(){
	return this->x;
}
int Platform::get_y() {
	return  this->y;
}
void Platform::update(SDL_Event& event){
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		
	}
	if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{

	}
	
}
void Platform::move_plat(){}
