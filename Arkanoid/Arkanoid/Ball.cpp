#include "Ball.h"

Ball::Ball(int x, int y){
	this->x = x;
	this->y = y;
	this->radius = 30;
	this->mPosX = this->x;
	this->mPosY = this->y;
	this->mVelX = 0;
	this->mVelY = 0;
	this->ballLaunched = false;
	SDL_Log("Ball()\n");
}

Ball::~Ball(){
	SDL_Log("~Ball()\n");
}

int Ball::get_x() {
	return this->x;
}

int Ball::get_y() {
	return this->y;
}

int Ball::get_rad() {
	return this->radius;
}

bool Ball::isBallLaunched() {

	return ballLaunched;
}

void Ball::draw(const TSharedPtr<renderer>& nRenderer) {
	nRenderer->DrawCircle(this->x, this->y, this->radius, 255, 20, 20, 255);
}


void Ball::movewithplatform(int plat_x, int plat_y, int plat_w, int plat_h) {

		this->x = plat_x + (plat_w / 2);
		this->y = plat_y - plat_h;

}

void Ball::move() {

}
