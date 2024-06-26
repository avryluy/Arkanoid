#pragma once
#include "lib\x64\SDL.h"
//#include "Engine\LTexture.h"
#include "Engine\Renderer.h"

class Platform; //Forward declaration

class Ball
{
public:
	Ball(int w, int h, float scale);
	~Ball();

	int scale_size(int dim, float scale);
	int get_x() const;
	int get_y() const;
	void set_y(int y);
	int get_w() const;
	int get_h() const;
	int get_rad() const;
	int get_life() const;
	int get_xDirection() const;
	int get_yDirection() const;

	bool isBallLaunched() const;
	bool ballCollideWall() const;
	void setBallBool();
	void winCon();
	bool lifeChanged() const;
	
	void move(Platform* platform, SDL_Rect mCol);
	void set_XDirection(int speed_x);
	void set_YDirection(int speed_y);
	//void movewithplatform(int plat_x, int plat_y, int plat_w, int plat_h);
	void update(SDL_Event& event);
	bool collision(SDL_Rect a, SDL_Rect b);
	void draw(const TSharedPtr<renderer>& nRenderer) const;
	void renderBall(const TSharedPtr<renderer>& nRenderer, int x, int y, int ballWidth, int ballHeight, SDL_Texture* mTexture, SDL_Rect* clip, float scale);
	

private:
	int x;
	int y;
	int mVelX;
	int mVelY;
	int mPosX;
	int mPosY;
	int radius;
	int speed = 8;
	int life = 3;
	int random_number = 1 + (std::rand() % static_cast<int>(10 - 1 + 1));
	bool ballLaunched;
	bool life_Changed;
	bool ball_bool;
	//LTexture* ballTexture = NULL;
	SDL_Rect bCol = {get_x(), get_y(), 40, 40};
};