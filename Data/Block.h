#pragma once

#include <Engine/Renderer.h>

class Block
{
public:
	Block();
	Block(int x, int y, int w, int h, int health, int blockID, int blockScore, int texture_id, float scale);
	//Block(int x, int y, SDL_Color blockColor, int health, int blockID, int blockScore);
	~Block();

	bool getCollision() const;
	void setCollision(bool col);
	bool isActive() const;
	
	int scale_size(int dim, float scale);

	int get_x() const;
	void set_x(int x);

	int get_texture_id() const;

	int get_y() const;
	void set_y(int y);

	int get_w() const;
	void set_w(int w);
	
	int get_h() const;
	void set_h(int h);

	int getHealth() const;
	int getStartHealth() const;

	int get_block_id() const;
	void set_block_id(int id);

	int get_block_score() const;

	void render(const TSharedPtr<renderer>& nRenderer, int x, int y, int blocWidth, int blocHeight, SDL_Texture* mTexture, SDL_Rect* clip) const;
	virtual void draw(const TSharedPtr<renderer>& nRenderer);
	void damage(int points);
	//void update();
	void Destroy();



private:
	bool ballCollision;
	bool blockActive;
	bool blockBool;
	
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

	SDL_Color blockColor = {0, 0, 0};

	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;

};


