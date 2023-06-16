#include "Engine\TextTextures.h"


TextTextures::TextTextures(std::string name)
{
	this->name = name;
	SDL_Log("%s()\n", name.c_str());
}

TextTextures::~TextTextures()
{
	SDL_Log("~%s()\n", name.c_str());
	closeText();

}


void TextTextures::DrawText(const TSharedPtr<renderer>& nRenderer, std::string textureText, SDL_Color textColor, int fontSize)
{
	mFont = TTF_OpenFont("C:/Users/avryl/Documents/CODE/Arkanoid/Arkanoid/Arkanoid/Helvetica-Bold.ttf", fontSize);
	if (mFont == NULL)
	{
		SDL_Log("Font failed to load. SDL_ttf error: %s\n", TTF_GetError());
	}
	else
	{
		freeTexture();
		SDL_Surface* textSurf = TTF_RenderText_Solid(mFont, textureText.c_str(), textColor);
		if (textSurf == NULL)
		{
			SDL_Log("Unable to render text surface: %s", TTF_GetError());
		}
		else
		{
			mTexture = SDL_CreateTextureFromSurface(nRenderer->GetNativeRenderer(), textSurf);
			if (mTexture == NULL)
			{
				SDL_Log("Unable to render texture: %s", TTF_GetError());
			}
			else
			{
				SDL_FreeSurface(textSurf);
			}
		}

	}
}

void TextTextures::freeTexture() {
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
	}
}


void TextTextures::renderText(const TSharedPtr<renderer>& nRenderer, int x, int y, int w, int h)
{
	SDL_Rect renderQuad = { x, y, w, h };
	SDL_RenderCopyEx(nRenderer->GetNativeRenderer(), mTexture, 0, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);

}

void TextTextures::closeText() {
	freeTexture();
	SDL_DestroyTexture(mTexture);
	TTF_Quit();
}