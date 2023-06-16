#pragma once
#include "SDL_ttf.h"
#include "SDL.h"
#include "Engine\Templates.h"
#include "Engine\Renderer.h"


class TextTextures
{
public:
	TextTextures(std::string name);
	~TextTextures();
	void freeTexture();
	void DrawText(const TSharedPtr<renderer>& nRenderer, std::string textureText, SDL_Color textColor, int fontSize);
	void renderText(const TSharedPtr<renderer>& nRenderer, int x, int y, int w, int h);
	void closeText();


private:
	SDL_Texture* mTexture = NULL;
	TTF_Font* mFont = NULL;
	std::string name;

};

