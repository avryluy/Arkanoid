#pragma once
#include "SDL.h"
#include "Engine\Renderer.h"


class Ball
{
public:
	Ball(int x, int y);
	~Ball();

	int get_x();
	int get_y();
	int get_rad();

	bool isBallLaunched();
	
	void move(int plat_x, int plat_y, int plat_w, int plat_h);
	void setDirection(int speed_x, int speed_y);
	//void movewithplatform(int plat_x, int plat_y, int plat_w, int plat_h);
	void update(SDL_Event& event);
	void draw(const TSharedPtr<renderer>& nRenderer);
	

private:
	int x;
	int y;
	int mVelX;
	int mVelY;
	int mPosX;
	int mPosY;
	int radius;
	int speed = 7;

	bool ballLaunched;
};