#pragma once

#include "lib\x64\SDL.h"
//#include "Engine\LTexture.h"
#include "Engine\Renderer.h"
#include "Data/GameConstants.h"

class Platform
{
public:
	Platform(int w, int h, float scale);
	~Platform();

	int get_x();
	int get_y();
	int get_w();
	int get_h();
	int get_plat_center();
	SDL_Rect get_collider();
	void draw_screen(const TSharedPtr<renderer>& nRenderer);
	void renderPlat(const TSharedPtr<renderer>& nRenderer, int x, int y, int platWidth, int platHeight, SDL_Texture* mTexture, SDL_Rect* clip, float scale);
	void update(SDL_Event &event);
	void move_plat(int boundary);

	int scale_size(int dim, float scale);
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
	bool plat_bool;

	SDL_Rect pCollider;
	//LTexture* ballTexture = NULL;

};


