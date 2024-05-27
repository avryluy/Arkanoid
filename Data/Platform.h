#pragma once

#include "SDL.h"
#include "Engine\Renderer.h"

class Platform
{
public:
	Platform();
	~Platform();

	int get_x();
	int get_y();
	int get_w();
	int get_h();
	int get_plat_center();
	SDL_Rect get_collider();
	void draw_screen(const TSharedPtr<renderer>& nRenderer);
	void update(SDL_Event &event);
	void move_plat(int boundary);
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

	SDL_Rect pCollider;
};


