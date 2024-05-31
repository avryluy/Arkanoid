#pragma once

#include <Engine/Renderer.h>

class Block
{
public:
	Block();
	Block(int x, int y, int w, int h, int health, int blockID, int blockScore, int texture_id, float scale);
	//Block(int x, int y, SDL_Color blockColor, int health, int blockID, int blockScore);
	~Block();

	bool getCollision();
	bool isActive();
	
	int scale_size(int dim, float scale);

	int get_x();
	void set_x(int x);

	int get_texture_id();

	int get_y();
	void set_y(int y);

	int get_w();
	void set_w(int w);
	
	int get_h();
	void set_h(int h);

	int getHealth();
	int getStartHealth();

	int get_block_id();
	void set_block_id(int id);

	int get_block_score();

	void render(const TSharedPtr<renderer>& nRenderer, int x, int y, int blocWidth, int blocHeight, SDL_Texture* mTexture, SDL_Rect* clip);
	virtual void draw(const TSharedPtr<renderer>& nRenderer);
	void damage(int points);
	//void update();
	void Destroy();



private:
	bool ballCollision;
	bool blockActive;
	
	int x;
	int y;
	int w;
	int h;
	int block_id;
	int texture_id;

	int padding = 10;

	int scoreValue;
	int startHealth;
	int health;

	SDL_Color blockColor;

	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;

};


