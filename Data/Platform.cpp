#include "Data\Platform.h"


Platform::Platform(int w, int h, float scale)
{
	this->x = 300;
	this->y = 560;
	this->w = scale_size(w, scale);
	this->h = scale_size(h, scale);

	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 255;
	SDL_Log("Platform()\n");
	mPosX = this->x;
	mVelX = 0;
	this->plat_bool = true;

	this->pCollider.w = scale_size(w, scale);
	this->pCollider.h = scale_size(h, scale);

}

Platform::~Platform()
{
	SDL_Log("~Platform()\n");

}

int Platform::scale_size(int dim, float scale)
{
	if (scale > 0)
	{
		int scaled = static_cast<int>(std::ceil(dim * scale));
		//printf("Prev: %i | Cur: %i\n", dim, scaled);
		return scaled;
	}
	else
	{
		//printf("Scale value was 0 or less.\n");
		return dim;
	}

}

void Platform::renderPlat(const TSharedPtr<renderer>& nRenderer, int x, int y, int platWidth, int platHeight, SDL_Texture* mTexture, SDL_Rect* clip, float scale) {
	SDL_Rect renderquad = { x, y, platWidth, platHeight };

	if (clip != NULL)
	{
		clip->w = renderquad.w;
		clip->h = renderquad.h;
	}

	//if (scale != 0.0)
	//{
	//	renderquad.w = renderquad.w * scale;
	//	renderquad.h = renderquad.h * scale;
	//}

	if (this->w != renderquad.w || this->h != renderquad.h)
	{
		renderquad.w = this->w;
		renderquad.h = this->h;
		this->pCollider.w = this->w;
		this->pCollider.h = this->h;

	}
	if (plat_bool)
	{
		printf("plat X: %i\n", this->x);
		printf("plat Y: %i\n", this->y);
		printf("plat W: %i\n", this->get_w());
		printf("plat H: %i\n", this->get_h());
		printf("plat col X: %i\n", this->pCollider.x);
		printf("plat col Y: %i\n", this->pCollider.y);
		printf("plat col W: %i\n", this->pCollider.w);
		printf("plat col H: %i\n", this->pCollider.h);
		plat_bool = false;
	}


	SDL_RenderCopy(nRenderer->GetNativeRenderer(), mTexture, clip, &renderquad);
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
	printf("Plat X: %i\n", this->x);
	printf("Plat W/2: %i\n", (this->w/2));
	printf("Plat Center: %i\n", plat_center);
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
	//this->pCollider.y = get_y();
	//SDL_Log("Plat Collider| X:%i | Y:%i", pCollider.x, pCollider.y);


	// Limits platform movement to just game screen
	if (mPosX < 1 || mPosX + this->w > boundary)
	{
		mPosX -= mVelX;
		this->x = mPosX;
	}

}

// Allows platform collider to be used with ball and other objects if needed
SDL_Rect Platform::get_collider() {
	
	return pCollider;
}
