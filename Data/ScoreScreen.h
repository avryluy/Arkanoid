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
	void draw_screen(const TSharedPtr<renderer>& nRenderer);

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

