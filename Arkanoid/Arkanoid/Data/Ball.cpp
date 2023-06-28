#include "Ball.h"
//TODO: These x y values for ball do nothing. remove.
Ball::Ball(int x, int y){
	this->x = x;
	this->y = y;
	this->radius = 20;
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

	return this->ballLaunched;
}

void Ball::draw(const TSharedPtr<renderer>& nRenderer) {
	nRenderer->DrawCircle(this->x, this->y, this->radius, 255, 20, 20, 255);
}

void Ball::setDirection(int speed_x, int speed_y) {
	this->mVelX = speed_x;
	this->mVelY = speed_y;
}

//void Ball::movewithplatform(int plat_x, int plat_y, int plat_w, int plat_h) {
//
//		this->x = plat_x + (plat_w / 2);
//		this->y = plat_y - plat_h;
//
//}

void Ball::move(int plat_x, int plat_y, int plat_w, int plat_h) {
	if (!ballLaunched)
	{
		this->x = plat_x + (plat_w / 2);
		this->y = (plat_y - plat_h) + (get_rad()/ 3);

	}
	
	mPosY += mVelY;
	this->y = mPosY;

	if ((mPosY < 0))
	{
		//Move back

		setDirection(0, (speed));
		mPosY += mVelY;
		this->y = mPosY;
	}
	else if ((mPosY + (this->radius * 2) > 720))
	{
		setDirection(0, (speed * -1));

	}
	SDL_Log("Ball velocity: %i", mVelY);

}

void Ball::update(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			this->ballLaunched = true;
			setDirection(0, (speed * -1));

		}
	}
}
