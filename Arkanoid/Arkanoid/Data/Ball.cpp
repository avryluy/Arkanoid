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

	this->bCol.w = (40);
	this->bCol.h = (100);
	this->ballLaunched = false;
	this->life_Changed = false;
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

int Ball::get_life() {
	return this->life;
}

bool Ball::isBallLaunched() {

	return this->ballLaunched;
}

void Ball::draw(const TSharedPtr<renderer>& nRenderer) {
	if (life <= 3 && life > 0)
	{
		nRenderer->DrawCircle(this->x, this->y, this->radius, 255, 20, 20, 255);
	}
	else
	{
		nRenderer->DrawCircle(this->x, this->y, this->radius, 0, 0, 0, 0);
	}
}

void Ball::set_XDirection(int speed_x) {
	this->mVelX = speed_x;
}

void Ball::set_YDirection(int speed_y) {
	this->mVelY = speed_y;
}


void Ball::move(int plat_x, int plat_y, int plat_w, int plat_h, SDL_Rect mCol) {
	//Moves ball with platform if not launched
	if (!ballLaunched)
	{
		mPosX = plat_x + (plat_w / 2);
		mPosY = (plat_y - plat_h) + (get_rad() / 3);

	}

	mPosX += mVelX;
	this->x = mPosX;

	//When ball hits left side of screen
	if (mPosX < 0)
	{
		set_XDirection(speed);
		mPosX += mVelX;
		bCol.x = mPosX;
		SDL_Log("Hit Left wall");
	}
	//When ball collides or hits right side of screen
	else if (mPosX + get_rad() > 720 || collision(bCol, mCol))
	{
		set_XDirection(-speed);
		mPosX += mVelX;
		bCol.x = mPosX;
		SDL_Log("Hit Right Wall OR Side of Platform");
	}

	bCol.x = mPosX;
	bCol.y = mPosY;

	//SDL_Log("Ball Collider| X:%i | Y:%i", bCol.x, bCol.y);

	mPosY += mVelY;
	this->y = mPosY;

	//When ball hits top of screen
	if (mPosY < 0)
	{
		//Move back
		SDL_Log("Hit Top of Screen");
		set_YDirection(speed);
		mPosY += mVelY;
		bCol.y = mPosY;
	}
	//When ball collides
	else if (collision(bCol, mCol))
	{
		SDL_Log("Hit Platform");
		//set_YDirection(-speed);
		int widthThirds = mCol.w / 3;
		int topLeftThird = (mCol.x + (mCol.x + widthThirds));
		int topMiddleThird = (mCol.x + (mCol.x + (widthThirds * 2)) );
		int topRightThird = (mCol.x + (mCol.x + mCol.w));
		int bCollPoint = bCol.y + bCol.h;
		
		//When ball hits left side of platform
		if ((bCollPoint >= mCol.x) & (bCollPoint < topLeftThird))
		{
			set_XDirection(-4);
			set_YDirection(-5);
			SDL_Log("Left Side Hit");
		}
		//When ball hits center of platform
		else if ((bCollPoint >= topLeftThird) & (bCollPoint < topMiddleThird))
		{
			set_XDirection(0);
			set_YDirection(-5);
			SDL_Log("Center Hit");
		}
		//When ball hits right side of platform
		// TODO: Not consistently registering Right side.
		else if ((bCollPoint >= topMiddleThird) & (bCollPoint <= topRightThird))
		{
			set_XDirection(4);
			set_YDirection(-5);
			SDL_Log("Right Side Hit");
		}
		mPosY += mVelY;
		bCol.y = mPosY;
	}
	//When ball hits bottom of screen
	else if ((mPosY + (this->radius) > 720))
	{
		// handles ball "death" and resetting
		SDL_Log("Hit Bottom of Screen");
		life_Changed = true;
		ballLaunched = false;
		mVelX = 0;
		mVelY = 0;
		this->life -= 1;
		SDL_Log("Lives left: %i", this->life);
		SDL_Log("Life Changed: %s", this->life_Changed ? "true" : "false");
	}

	//SDL_Log("Ball velocity: %i", mVelY);

}

void Ball::update(SDL_Event& event) {
	// Hitting Space launches ball
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		if (event.key.keysym.sym == SDLK_SPACE && !ballLaunched)
		{
		
			this->ballLaunched = true;
			set_XDirection(random_number);
			set_YDirection(speed);
			life_Changed = false;
		}
	}
}

bool Ball::collision(SDL_Rect a ,SDL_Rect b) {

	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calc sides of rectA
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calc sides of rectB
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	// checks for side collisions
	if (bottomA  <= topB)
	{
		return false;
	}
	if (bottomB >= topA)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

bool Ball::lifeChanged() {
	if (life_Changed)
	{
		return true;
	}
	else
	{
		return false;
	}
}