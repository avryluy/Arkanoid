#pragma once
#include "lib\x64\SDL.h"
#include "Engine\Renderer.h"

class Platform; //Forward declaration

class Ball
{
public:
	Ball(int x, int y);
	~Ball();

	int get_x();
	int get_y();
	int get_w();
	int get_h();
	int get_rad();
	int get_life();

	bool isBallLaunched();
	bool lifeChanged();
	
	void move(Platform* platform, SDL_Rect mCol);
	void set_XDirection(int speed_x);
	void set_YDirection(int speed_y);
	//void movewithplatform(int plat_x, int plat_y, int plat_w, int plat_h);
	void update(SDL_Event& event);
	bool collision(SDL_Rect a, SDL_Rect b);
	void draw(const TSharedPtr<renderer>& nRenderer);
	

private:
	int x;
	int y;
	int mVelX;
	int mVelY;
	int mPosX;
	int mPosY;
	int radius;
	int speed = 5;
	int life = 3;
	int random_number = 1 + (std::rand() % static_cast<int>(10 - 1 + 1));
	bool ballLaunched;
	bool life_Changed;

	SDL_Rect bCol = {get_x(), get_y(), 40, 40};
};