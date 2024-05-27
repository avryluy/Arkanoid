#include "Data\Platform.h"


Platform::Platform()
{

	this->x = 300;
	this->y = 560;
	this->w = 200;
	this->h = 25;

	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 255;
	SDL_Log("Platform()\n");
	mPosX = this->x;
	mVelX = 0;

	this->pCollider.w = w;
	this->pCollider.h = h;

}

Platform::~Platform()
{
	SDL_Log("~Platform()\n");

}



void Platform::draw_screen(const TSharedPtr<renderer>& nRenderer)
{
	SDL_Rect mRect;
	mRect.x = this->x;
	mRect.y = this->y;
	mRect.w = this->w;
	mRect.h = this->h;
	nRenderer->DrawRect(mRect.x, mRect.y, mRect.w, mRect.h, this->r, this->g, this->b, this->a);

}

int Platform::get_x(){
	return this->x;
}

int Platform::get_y() {
	return  this->y;
}

int Platform::get_w() {
	return  this->w;
}

int Platform::get_h() {
	return  this->h;
}

int Platform::get_plat_center() {
	int plat_center;
	plat_center = (this->x + (this->w /2));
	return plat_center;

}

//Keyboard handler for platform movement
void Platform::update(SDL_Event& event){
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			//SDL_Log("Left key pressed");
			//SDL_Log("Platform X :%i Y:%i", get_x(), get_y());
			mVelX -= speed;
			break;
		case SDLK_RIGHT:
			//SDL_Log("Right key pressed");
			//SDL_Log("Platform X :%i Y:%i", get_x(), get_y());
			mVelX += speed;
			break;
		default:
			break;
		}
	}
	if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			//SDL_Log("Left key released");
			//SDL_Log("Platform X :%i Y:%i", get_x(), get_y());
			mVelX += speed;
			break;
		case SDLK_RIGHT:
			//SDL_Log("Right key released");
			//SDL_Log("Platform X :%i Y:%i", get_x(), get_y());
			mVelX -= speed;
			break;

		default:
			break;
		}

	}
}


void Platform::move_plat(int boundary){
	mPosX += mVelX;
	this->x = mPosX;
	this->pCollider.x = mPosX;
	this->pCollider.y = get_y() - get_h() - 15;
	//SDL_Log("Plat Collider| X:%i | Y:%i", pCollider.x, pCollider.y);


	// Limits platform movement to just game screen
	if (mPosX < 0 || mPosX + this->h > boundary)
	{
		mPosX -= mVelX;
		this->x = mPosX;
	}

}

// Allows platform collider to be used with ball and other objects if needed
SDL_Rect Platform::get_collider() {
	
	return pCollider;
}