#include "Engine\Renderer.h"


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

void renderer::DrawCircle(int x, int y, int radius, int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(NativeRenderer, r, g, b, a);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w;
			int dy = radius - h;
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(NativeRenderer, x + dx, y + dy);
			}
		}
	}

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