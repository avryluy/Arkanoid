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
	this->health = 0;

	this->canDestroy = false;
	this->ballCollision = false;
}

Block::Block(int r, int g, int b, int health)
{

	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;

	this->scoreValue = 0;
	this->health = health;

	this->canDestroy = false;
	this->ballCollision = false;
}

Block::~Block()
{
}

bool Block::getDestroy() {
	return this->canDestroy;
}

bool Block::getCollision() {
	return this->ballCollision;
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


void Block::draw(const TSharedPtr<renderer>& nRenderer) {
	SDL_Rect mRect;
	mRect.x = this->x;
	mRect.y = this->y;
	mRect.w = this->w;
	mRect.h = this->h;
	nRenderer->DrawRect(mRect.x, mRect.y, mRect.w, mRect.h, this->r, this->g, this->b, this->a);
}