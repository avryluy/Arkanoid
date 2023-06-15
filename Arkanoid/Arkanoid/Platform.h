#pragma once

#include "SDL.h"
#include "Renderer.h"

class Platform
{
public:
	Platform();
	~Platform();

	int get_x();
	int get_y();
	void draw_screen(const TSharedPtr<renderer>& nRenderer);
	void update(SDL_Event &event);
	void move_plat();
private:
	int x;
	int y;
	int w;
	int h;

	int mVelX;
	int mPosX;
	int speed = 10;

	int r;
	int g;
	int b;
	int a;

};


