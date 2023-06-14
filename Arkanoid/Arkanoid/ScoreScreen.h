#pragma once
#include "SDL.h"
#include "Renderer.h"

class ScoreScreen
{
public:
	ScoreScreen();
	~ScoreScreen();

	//int get_x();
	//int get_y();
	//void set_position(int x, int y);
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

	//TSharedPtr<renderer>& nRenderer;
};

