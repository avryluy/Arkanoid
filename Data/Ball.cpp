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
	this->ball_bool = false;
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


int Ball::get_x() const {
	return this->x;
}

int Ball::get_y() const {
	return this->y;
}

void Ball::set_y(int y) {
	this->y = y;
}

int Ball::get_w() const {
	return get_rad() * 2;
}

int Ball::get_h() const {
	return get_rad() * 2;
}

int Ball::get_rad() const {
	return this->radius;
}

int Ball::get_life() const {
	return this->life;
}

bool Ball::isBallLaunched() const {

	return this->ballLaunched;
}

void Ball::draw(const TSharedPtr<renderer>& nRenderer) const {
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
	int platTopLeft = (plat_x + platWidthThird)-5;
	int platTopMiddle = (platTopLeft + platWidthThird);
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
	int xDirectionChange = 0;
	int yDirectionChange = 0;

	// Handle X-Axis Collisions
	if (mPosX < 0) {
		set_XDirection(get_xDirection() + speed + (int)abs(random_number / 3));
		this->ball_bool = true;
		SDL_Log("Hit Left side of screen");
	}
	else if (ballRight > 720) {
		mPosX -= int(get_rad() * .5);
		set_XDirection(get_xDirection() - speed - (int)abs(random_number / 3));
		this->ball_bool = true;

		SDL_Log("Hit Right side of screen");
	}

	int framesToChange = 3; // Adjust as needed
	int xSpeedIncrement = xDirectionChange / framesToChange;
	int ySpeedIncrement = yDirectionChange / framesToChange;

	for (int i = 0; i < framesToChange; ++i) {
		set_XDirection(get_xDirection() + xSpeedIncrement);
		set_YDirection(get_yDirection() + ySpeedIncrement);

	}

	// Ensure the speed doesn't drop below a minimum threshold
	int minSpeed = 5; // Adjust as needed
	if (abs(get_xDirection()) < minSpeed) {
		set_XDirection(get_xDirection() > 0 ? minSpeed : -minSpeed);
	}
	if (abs(get_yDirection()) < minSpeed) {
		set_YDirection(get_yDirection() > 0 ? minSpeed : -minSpeed);
	}

	// Handle Y-Axis collisions
	if (mPosY < 0) {
		int yDirectionChange = speed + (int)abs(random_number / 3);
		mPosY += int(get_rad() * .5);
		this->ball_bool = true;
		SDL_Log("Hit Top of screen");
	}
	else if (collision(bCol, mCol)) {
		if (ballBottom >= (plat_y - (plat_h / 2)) && ballBottom <= (plat_y + (plat_h / 2))) {
			this->ball_bool = true;
			if (mPosX < platTopLeft) {
				xDirectionChange = -speed + (int)abs(random_number / 2);
				yDirectionChange = -speed - (int)abs(random_number / 3);
				mPosY -= int(get_rad() * .5);
				SDL_Log("Hit Left Side of Platform");
			}
			else if (mPosX >= platTopLeft && mPosX < platTopMiddle) {
				xDirectionChange = 4 + (int)abs(random_number / 2);
				yDirectionChange = -speed - (int)abs(random_number / 2);
				mPosY -= int(get_rad() * .5);
				SDL_Log("Hit Center of Platform");
			}
			else {
				xDirectionChange = speed - (int)abs(random_number / 2);
				yDirectionChange = -speed - (int)abs(random_number / 3);
				mPosY -= int(get_rad() * .5);
				SDL_Log("Hit Right Side of Platform");
			}

			// Gradually adjust the speed over a few frames
			xSpeedIncrement = xDirectionChange / framesToChange;
			ySpeedIncrement = yDirectionChange / framesToChange;

			for (int i = 0; i < framesToChange; ++i) {
				set_XDirection(get_xDirection() + xSpeedIncrement);
				set_YDirection(get_yDirection() + ySpeedIncrement);

			}

			// Ensure the speed doesn't drop below a minimum threshold
			if (abs(get_xDirection()) < minSpeed) {
				set_XDirection(get_xDirection() > 0 ? minSpeed : -minSpeed);
			}
			if (abs(get_yDirection()) < minSpeed) {
				set_YDirection(get_yDirection() > 0 ? minSpeed : -minSpeed);
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

bool Ball::ballCollideWall() const
{
	return this->ball_bool;
}

void Ball::setBallBool() {
	this->ball_bool = !ball_bool;
}

int Ball::get_xDirection() const {
	if (this->mVelX < 0)
	{
		return -1;
	}
	else if (this->mVelX > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Ball::get_yDirection() const {
	if (this->mVelY < 0)
	{
		return -1;
	}
	else if (this->mVelY > 0)
	{
		return 1;
	}
	else
	{
		return 0;
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

bool Ball::lifeChanged() const {
	if (life_Changed)
	{
		return true;
	}
	else
	{
		return false;
	}
}