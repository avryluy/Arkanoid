#pragma once
#include "lib\x64\SDL.h"
#include "Engine\Renderer.h"

class ScoreScreen
{
public:
	ScoreScreen();
	~ScoreScreen();

	int get_x();
	int get_y();
	int get_w();
	int get_h();
	void draw_screen(const TSharedPtr<renderer>& nRenderer);
	void render(const TSharedPtr<renderer>& nRenderer, int x, int y, int txtrWidth, int txtrHeight, SDL_Texture* mTexture, SDL_Rect* clip);

private:
	int x;
	int y;
	int w;
	int h;
	int r;
	int g;
	int b;
	int a;

};

