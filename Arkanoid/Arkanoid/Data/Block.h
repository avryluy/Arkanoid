#pragma once

#include "Engine\Renderer.h"

class Block
{
public:
	Block();
	Block(int r, int g, int b, int health);
	~Block();

	bool getDestroy();
	bool getCollision();
	bool isActive();
	
	int get_x();
	void set_x(int x);

	int get_y();
	void set_y(int y);

	int get_w();
	void set_w(int w);
	
	int get_h();
	void set_h(int h);

	virtual void draw(const TSharedPtr<renderer>& nRenderer);
	//void update();
	void Destroy();



private:
	bool canDestroy;
	bool ballCollision;
	bool blockActive;
	
	int x;
	int y;
	int w;
	int h;

	int padding = 10;

	int scoreValue;
	int health;

	int r;
	int g;
	int b;
	int a;

};


