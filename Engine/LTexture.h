#pragma once
#include "lib\x64\SDL.h"
#include "Engine\Templates.h"
#include "Engine\Renderer.h"

class LTexture {

	public:
		LTexture();
		~LTexture();


		bool loadImage(const TSharedPtr<renderer>& nRenderer, std::string path);
		void freeTexture();

		void render(const TSharedPtr<renderer>& nRenderer, int x, int y, SDL_Rect* clip = NULL);
		int getWidth() const;
		int getHeight() const;

		SDL_Texture* getTexture();

	private:
		SDL_Texture* mTexture = NULL;
		std::string path;
		int mWidth;
		int mHeight;

};
