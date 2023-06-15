#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Templates.h"
#include <iostream>
#include<stdio.h>

class renderer
{
public:
	//renderer();
	explicit renderer(struct SDL_Renderer* RendererPtr);
	~renderer();

	static TSharedPtr<renderer> Construct(struct SDL_Window* Window);


	void clear(int r, int g, int b, int a);
	void Present();

	void DrawRect(int x, int y, int w, int h, int r, int g, int b, int a);
	//void FillRect(int r, int g, int b, int a, struct SDL_Rect& rect);

	void DrawText(std::string textureText, SDL_Color textColor, int x, int y);

	//void DrawCirle();
	//void FillCirle(int r, int g, int b, int a);

	//void DrawSurface();
	//void FillSurface(int r, int g, int b, int a, );


	struct SDL_Renderer* GetNativeRenderer() const 
	{ 
		return NativeRenderer;
	};

private:
	struct SDL_Renderer* NativeRenderer = nullptr;
	SDL_Texture* mTexture = NULL;
	TTF_Font* mFont = NULL;
};
