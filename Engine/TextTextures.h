#pragma once
#include "lib\x64\SDL_ttf.h"
#include "lib\x64\SDL.h"
#include "Engine\Templates.h"
#include "Engine\Renderer.h"


class TextTextures
{
public:
	TextTextures(std::string name);
	~TextTextures();
	void freeTexture();
	void DrawText(const TSharedPtr<renderer>& nRenderer, std::string textureText, SDL_Color textColor, int fontSize);
	//void DrawText(const TSharedPtr<renderer>& nRenderer, update_text(), SDL_Color textColor, int fontSize);
	void renderText(const TSharedPtr<renderer>& nRenderer, int x, int y, int w, int h);
	void update_text(std::string input_text);
	void closeText();


private:
	SDL_Texture* mTexture = NULL;
	TTF_Font* mFont = NULL;
	std::string name;
	//std::string texturetext;

};

