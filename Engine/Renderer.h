#pragma once

#include "lib\x64\SDL.h"
#include "Engine\Templates.h"
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

	void renderBackground(SDL_Texture* bTexture, SDL_Rect* clip, int screenWidth, int screenHieght);

	void DrawCircle(int x, int y, int radius, int r, int g, int b, int a);
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
};
