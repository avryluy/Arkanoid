#include "Renderer.h"


TSharedPtr<renderer> renderer::Construct(struct SDL_Window* window){
	SDL_Renderer* NativeRenderer;
	NativeRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!NativeRenderer)
	{
		SDL_Log("Unable to load Renderer: %s", SDL_GetError());
		return nullptr;
	}
	SDL_Log("renderer created\n");
	return std::make_shared<renderer>(NativeRenderer);


}

renderer::renderer(SDL_Renderer* renderer) : NativeRenderer(renderer)
{
	SDL_SetRenderDrawBlendMode(NativeRenderer, SDL_BLENDMODE_BLEND);
}

renderer::~renderer()
{
	if (NativeRenderer)
	{
		SDL_DestroyRenderer(NativeRenderer);
		NativeRenderer = NULL;
		SDL_Log("~renderer()\n");
	}

}


void renderer::clear(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(NativeRenderer, r, g, b, a);
	SDL_RenderClear(NativeRenderer);
}

void renderer::Present()
{
	SDL_RenderPresent(NativeRenderer);
}

void renderer::DrawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_Rect mRect;
	mRect.x = x;
	mRect.y = y;
	mRect.w = w;
	mRect.h = h;
	SDL_SetRenderDrawColor(NativeRenderer, r, g, b, a);
	SDL_RenderFillRect(NativeRenderer, &mRect);
	SDL_RenderDrawRect(NativeRenderer, &mRect);

}

void renderer::DrawText(std::string textureText, SDL_Color textColor, int x, int y)
{
	mFont = TTF_OpenFont("C:/Users/avryl/Documents/CODE/Arkanoid/Arkanoid/Arkanoid/Helvetica-Bold.ttf", 120);
	if (mFont == NULL)
	{
		SDL_Log("Font failed to load. SDL_ttf error: %s\n", TTF_GetError());
	}
	SDL_Surface* textSurf = TTF_RenderText_Solid(mFont, textureText.c_str(), textColor);
	mTexture = SDL_CreateTextureFromSurface(NativeRenderer, textSurf);
	SDL_Rect renderQuad = { x, y, 350, 100};
	//SDL_Rect clip = {10, 10, 300, 300};


	SDL_RenderCopyEx(NativeRenderer, mTexture, 0, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);


	
}

//void renderer::DrawSurface() {
//
//	SDL_Surface mSurface;
//	//SDL_Surfe
//
//}

//void renderer::FillRect(int r, int g, int b, int a, struct SDL_Rect& rect)
//{
//	SDL_RenderFillRect(NativeRenderer, &rect);
//}