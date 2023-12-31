#include "Block.h"

Block::Block()
{

	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;

	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 0;

	this->scoreValue = 0;
	this->health = 1;

	this->ballCollision = false;
	this->blockActive = true;
}

Block::Block(int x, int y, SDL_Color blockColor, int health, int blockID)
{
	this->x = x;
	this->y = y;
	this->w = 70;
	this->h = 30;
	
	this->blockColor = blockColor;

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;

	this->scoreValue = 0;
	this->block_id = blockID;
	this->health = health;

	this->ballCollision = false;
	this->blockActive = true;
}

Block::~Block()
{
	this->blockActive = false;

}

bool Block::getCollision() {
	return this->ballCollision;
}

bool Block::isActive() {
	return blockActive;
}

int Block::get_x() {
	return this->x;
}

int Block::get_y() {
	return this->y;
}

int Block::get_w() {
	return this->w;
}

int Block::get_h() {
	return this->h;
}

int Block::getHealth() {
	return this->health;
}

int Block::get_block_id() {
	return this->block_id;
}
void Block::set_block_id(int id) {
	this->block_id = id;
}

void Block::set_y(int y) {
	this->y = y;
}

void Block::set_x(int x) {
	this->x = x + padding;
}

void Block::set_w(int w) {
	this->w = w;
}

void Block::set_h(int h) {
	this->h = h;
}

void Block::damage(int points) {

	this->health = this->health - points;
}


void Block::draw(const TSharedPtr<renderer>& nRenderer) {
	SDL_Rect mRect;
	mRect.x = this->x;
	mRect.y = this->y;
	mRect.w = this->w;
	mRect.h = this->h;
	if (this->health > 0 ) {
		nRenderer->DrawRect(mRect.x, mRect.y, mRect.w, mRect.h, this->blockColor.r, this->blockColor.g, this->blockColor.b, this->blockColor.a);
	}
	
}

void Block::Destroy() {
	this->blockActive = false;
}

