#include "Ball.h"
#include "Platform.h"
//TODO: These x y values for ball do nothing. remove.
Ball::Ball(int w, int h, float scale) {
	this->x = 0;
	this->y = 0;

	this->bCol.w = scale_size(w, scale);
	this->bCol.h = scale_size(h, scale);

	this->radius = static_cast<int>(scale_size(w, scale) * .5);
	this->mPosX = this->x;
	this->mPosY = this->y;
	this->mVelX = 0;
	this->mVelY = 0;

	this->ballLaunched = false;
	this->life_Changed = false;
	this->ball_bool = true;
	SDL_Log("Ball()\n");
}

Ball::~Ball(){
	SDL_Log("~Ball()\n");
}

int Ball::scale_size(int dim, float scale)
{
	if (scale > 0)
	{
		int scaled = static_cast<int>(std::ceil(dim * scale));
		//printf("Prev: %i | Cur: %i\n", dim, scaled);
		return scaled;
	}
	else
	{
		printf("Scale value was 0 or less.\n");
		return dim;
	}

}


int Ball::get_x() {
	return this->x;
}

int Ball::get_y() {
	return this->y;
}

void Ball::set_y(int y) {
	this->y = y;
}

int Ball::get_w() {
	return get_rad() * 2;
}

int Ball::get_h() {
	return get_rad() * 2;
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

void Ball::renderBall(const TSharedPtr<renderer>& nRenderer, int x, int y, int ballWidth, int ballHeight, SDL_Texture* mTexture, SDL_Rect* clip, float scale)
{
	SDL_Rect renderquad = { x, y, ballWidth, ballHeight };

	if (clip != NULL) {

		clip->w = renderquad.w;
		clip->h = renderquad.h;
	}

	//if (scale != 0.0)
	//{
	//	renderquad.w = renderquad.w * scale;
	//	renderquad.h = renderquad.h * scale;
	//}

	if (this->bCol.w != renderquad.w || this->bCol.h != renderquad.h)
	{
		renderquad.w = this->bCol.w;
		renderquad.h = this->bCol.h;
		//this->radius = std::ceil((renderquad.x + renderquad.w) * 0.5f);
	}
	if(ball_bool)
	{
		printf("Ball radius: %i\n", this->radius);
		printf("Ball X: %i\n", this->x);
		printf("Ball Y: %i\n", this->y);
		printf("Ball W: %i\n", this->get_w());
		printf("Ball H: %i\n", this->get_h());
		printf("Ball col X: %i\n", this->bCol.x);
		printf("Ball col Y: %i\n", this->bCol.y);
		printf("Ball col W: %i\n", this->bCol.w);
		printf("Ball col H: %i\n", this->bCol.h);
		ball_bool = false;
	}

	SDL_RenderCopy(nRenderer->GetNativeRenderer(), mTexture, clip, &renderquad);

}


void Ball::set_XDirection(int speed_x) {
	this->mVelX = speed_x;
}

void Ball::set_YDirection(int speed_y) {
	this->mVelY = speed_y;
}

void Ball::move(Platform* platform, SDL_Rect mCol) {
	// Declare variables
	int plat_x = platform->get_x();
	int plat_y = platform->get_y();
	int plat_w = platform->get_w();
	int plat_h = platform->get_h();
	int platWidthThird = plat_w / 3;
	int platTopLeft = plat_x + platWidthThird;
	int platTopMiddle = plat_x + 2 * platWidthThird;
	int ballBottom = mPosY + this->radius;
	int ballRight = mPosX + this->radius;

	// If ball isn't launched
	if (!ballLaunched) {
		this->life_Changed = false;
		mPosX = (int)(plat_x + (plat_w * .38));
		mPosY = (int)((plat_y - plat_h) - 2);
	}

	// Ball Movement
	mPosX += mVelX;
	this->x = mPosX;
	mPosY += mVelY;
	this->y = mPosY;

	// Handle X-Axis Collisions
	if (mPosX < 0) {
		set_XDirection(speed);
		SDL_Log("Hit Left side of screen");
	}
	else if (ballRight > 720) {
		set_XDirection(-speed);
		SDL_Log("Hit Right side of screen");
	}

	// Handle Y-Axis collisions
	if (mPosY < 0) {
		set_YDirection(speed);
		SDL_Log("Hit Top of screen");
	}
	else if (collision(bCol, mCol)) {
		if (ballBottom >= (plat_y - (plat_h / 2)) && ballBottom <= (plat_y + (plat_h / 2))) {
			if (mPosX < platTopLeft) {
				set_XDirection(-speed);
				set_YDirection(-speed);
				SDL_Log("Hit Left Side of Platform");
			}
			else if (mPosX >= platTopLeft && mPosX < platTopMiddle) {
				set_XDirection(0 + random_number);
				set_YDirection(-speed);
				SDL_Log("Hit Center of Platform");
			}
			else {
				set_XDirection(speed);
				set_YDirection(-speed);
				SDL_Log("Hit Right Side of Platform");
			}
		}
	}
	else if (ballBottom > 720 && mVelY > 0) {
		SDL_Log("Hit Bottom of Screen");
		life_Changed = true;
		ballLaunched = false;
		mVelX = 0;
		mVelY = 0;
		this->life -= 1;
		SDL_Log("Lives left: %i", this->life);
		SDL_Log("Life Changed: %s", this->life_Changed ? "true" : "false");
	}

	// Update Collider Position
	bCol.x = mPosX;
	bCol.y = mPosY;
}


//void Ball::move(Platform* platform, SDL_Rect mCol) {
//	//declare variables
//	int plat_x = platform->get_x();
//	int plat_y = platform->get_y();
//	int plat_w = platform->get_w();
//	int plat_h = platform->get_h();
//	int platWidthThird = plat_w / 3;
//	int platTopLeft = plat_x + platWidthThird;
//	int platTopMiddle = plat_x + 2 * platWidthThird;
//	int ballBottom = mPosY + this->radius;
//	int ballRight = mPosX + this->radius;
//	
//	//if ball isnt launched
//	if (!ballLaunched)
//	{
//		mPosX = plat_x + (plat_w * .38);
//		mPosY = (plat_y - plat_h) - 2;
//	}
//
//	//Ball Movement
//	mPosX += mVelX;
//	this->x = mPosX;
//	mPosY += mVelY;
//	this->y = mPosY;
//
//	//Handle X-Axis Collisions
//	if (mPosX < 0) {
//		set_XDirection(speed);
//		SDL_Log("Hit Left side of screen");
//	}
//	else if (ballRight > 720 ) {
//		//Ball hitting right wall or platform
//		set_XDirection(-speed);
//		//set_YDirection(-(random_number));
//		SDL_Log("Hit Right side of screen");
//	}
//
//	//SDL_Log("Ball X Speed: %i", mVelX);
//	//SDL_Log("Ball Y Speed: %i", mVelY);
//
//	//Update Collider Position
//	bCol.x = mPosX;
//	bCol.y = mPosY;
//		
//	//Handle Y-Axis collisions
//	if (mPosY < 0) {
//		//Ball hitting top wall
//		set_YDirection(speed);
//		SDL_Log("Hit Top of screen");
//	}
//	else if (collision(bCol, mCol)) {
//		//Ball hitting platform
//		if (ballBottom >= (plat_y - (plat_h/2) ) && ballBottom <= (plat_y + (plat_h/2) ) ) {
//			if (mPosX < platTopLeft) {
//				//Ball hits left side of platform
//				set_XDirection(-speed);
//				set_YDirection(-speed);
//				SDL_Log("Hit Left Side of Platform");
//			}
//			else if (mPosX >= platTopLeft && mPosX < platTopMiddle) {
//				//Ball hits center of platform
//				set_XDirection(0 + random_number);
//				set_YDirection(-speed);
//				SDL_Log("Hit Center of Platform");
//			}
//			else {
//				//Ball hits right side of platform
//				set_XDirection(speed);
//				set_YDirection(-speed);
//				SDL_Log("Hit Right Side of Platform");
//			}
//		}
//
//	}
//	else if (ballBottom > 720 && mVelY > 0) {
//		//Ball hitting bottom wall
//		SDL_Log("Hit Bottom of Screen");
//		life_Changed = true;
//		ballLaunched = false;
//		mVelX = 0;
//		mVelY = 0;
//		this->life -= 1;
//		SDL_Log("Lives left: %i", this->life);
//		SDL_Log("Life Changed: %s", this->life_Changed ? "true" : "false");
//
//	}
//
//	mPosX += mVelX;
//	mPosY += mVelY;
//}

void Ball::update(SDL_Event& event) {
	// Hitting Space launches ball
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		if (event.key.keysym.sym == SDLK_SPACE && !ballLaunched)
		{
		
			this->ballLaunched = true;
			set_XDirection(random_number);
			set_YDirection(-speed);
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