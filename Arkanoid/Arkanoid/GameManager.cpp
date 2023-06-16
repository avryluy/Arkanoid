#include "GameManager.h"


GameManager::GameManager()
{
	scorescreen = new ScoreScreen();
	platform = new Platform();
}

GameManager::~GameManager()
{
	Renderer = NULL;
}

int GameManager::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		TTF_Init();
	};
	//Create Renderer
	Renderer = renderer::Construct(window);
	main_surf = SDL_GetWindowSurface(window);
	Renderer->DrawText("Arkanoid", { 0, 0, 0 }, 200);
	//Start gameLoop
	GameLoop();
	return 0;
}

void GameManager::GameLoop()
{
	FPSManager fps;
	fps.start_time();
	FPSManager cfps;
	gameRunning = true;
	countedframes = 0;
	while (gameRunning)
	{

		cfps.start_time();
		cfps.frame_avg(countedframes, fps.get_time());
		//SDL_Log("Countedframes %i", countedframes);
		//SDL_Log("GetTime %i", fps.get_time());
		HandleEvents();
		platform->move_plat(scorescreen->get_x() - 150);
		Render(Renderer);
		++countedframes;
		cfps.frame_limit(cfps.get_time());

	};
	
	Quit();
}

void GameManager::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			gameRunning = false;
		}
		platform->update(event);
	}
}

void GameManager::Render(const TSharedPtr<renderer>& nRenderer)
{
	nRenderer->clear(88, 154, 186, 255);
	platform->draw_screen(nRenderer);
	scorescreen->draw_screen(nRenderer);
	nRenderer->renderText((scorescreen->get_x() + 10), (scorescreen->get_y() + 30), 350, 100);
	nRenderer->Present();
}

void GameManager::Quit()
{
	Renderer->~renderer();
	delete scorescreen;
	scorescreen = NULL;
	SDL_Quit();
}