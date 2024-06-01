#include "Engine/LTexture.h"
#include "lib/x64/SDL_image.h"
#include <stdio.h>
#include <string>

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	printf("LTexture values --- w: %i | h: %i\n", mWidth, mHeight);
};

LTexture::~LTexture() {

	freeTexture();
};


bool LTexture::loadImage(const TSharedPtr<renderer>& nRenderer, std::string path) {
	freeTexture();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* mSurface = IMG_Load(path.c_str());
	if (mSurface == NULL)
	{
		printf("Cannot load image: %s\n", path.c_str());
		exit(EXIT_FAILURE);
	}
	else {

		SDL_SetColorKey(mSurface, SDL_TRUE, SDL_MapRGB(mSurface->format, 0, 0, 0));

		newTexture = SDL_CreateTextureFromSurface(nRenderer->GetNativeRenderer(), mSurface);
		if (newTexture == NULL)
		{
			printf("Cannot load texture\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Settings w/h vals\n");
			mWidth = mSurface->w;
			mHeight = mSurface->h;
			//printf("LTexture values --- w: %i | h: %i\n", mWidth, mHeight);

		}
		SDL_FreeSurface(mSurface);
	}
	
	mTexture = newTexture;
	return mTexture != NULL;

};

SDL_Texture* LTexture::getTexture() {

	return mTexture;
}


void LTexture::freeTexture() {
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
};

void LTexture::render(const TSharedPtr<renderer>& nRenderer, int x, int y, SDL_Rect* clip) { 
	SDL_Rect renderquad = { x, y, mWidth, mHeight };

	if (clip != NULL) {

		clip->w = renderquad.w;
		clip->h = renderquad.h;
	}

	SDL_RenderCopy(nRenderer->GetNativeRenderer(), mTexture, clip, &renderquad);

};

int LTexture::getWidth() {
	return mWidth;
};

int LTexture::getHeight() {
	return mHeight;
};