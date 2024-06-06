#include <Engine\TextTextures.h>


TextTextures::TextTextures(std::string name)
{
	this->name = name;
	//SDL_Log("%s()\n", name.c_str());
}

TextTextures::~TextTextures()
{
	//SDL_Log("~%s()\n", name.c_str());
	closeText();

}


void TextTextures::DrawText(const TSharedPtr<renderer>& nRenderer, std::string textureText, SDL_Color textColor, int fontSize)
{
	//Load font
	mFont = TTF_OpenFont("Assets\\Pixel Digivolve.otf", fontSize);
	if (mFont == NULL)
	{
		SDL_LogError(6, "Font failed to load. SDL_ttf error: %s\n", TTF_GetError());
	}
	else
	{
		//Create Texture for Text
		freeTexture();
		SDL_Surface* textSurf = TTF_RenderText_Solid(mFont, textureText.c_str(), textColor);
		if (textSurf == NULL)
		{
			SDL_LogError(6, "Unable to render text surface: %s", TTF_GetError());
		}
		else
		{
			mTexture = SDL_CreateTextureFromSurface(nRenderer->GetNativeRenderer(), textSurf);
			if (mTexture == NULL)
			{
				SDL_LogError(6, "Unable to render texture: %s", TTF_GetError());
			}
			else
			{
				SDL_FreeSurface(textSurf);
				TTF_CloseFont(mFont);
			}
		}

	}
}

void TextTextures::update_text(std::string input_text) {

	std::string fontvalue = input_text;
	//DrawText()

}

void TextTextures::freeTexture() {
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
	}
}


void TextTextures::renderText(const TSharedPtr<renderer>& nRenderer, int x, int y, int w, int h)
{
	// render to screen
	SDL_Rect renderQuad = { x, y, w, h };
	SDL_RenderCopyEx(nRenderer->GetNativeRenderer(), mTexture, 0, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);

}

void TextTextures::closeText() {
	//Destroys text
	freeTexture();
	SDL_DestroyTexture(mTexture);
	TTF_Quit();
}