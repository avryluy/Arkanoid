#include "Engine\GameManager.h"


GameManager::GameManager()
{
	//Construct objects for game
	scorescreen = new ScoreScreen();
	platform = new Platform();
	text = new TextTextures("Title");
	subtext = new TextTextures("Sub-title");
	life_count = new TextTextures("Life Counter");
	//ball = new Ball(platform->get_plat_center(), platform->get_y() - 20);
	//Block *blocks = new Block(0, 255, 10, 1);
	Init_Level();
	audioman = new AudioManager("C:\\Users\\avryl\\Documents\\CODE\\Arkanoid\\Arkanoid\\Arkanoid\\Audio\\bow_shot_02.wav");
	//audioman->loadFile(testfile);
}

GameManager::~GameManager()
{
	Renderer = NULL;
}

int GameManager::Init() {
	//Initialize Game window, renderer, and text
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		TTF_Init();
	};
	//Create Renderer
	Renderer = renderer::Construct(window);
	main_surf = SDL_GetWindowSurface(window);
	text->DrawText(Renderer,"Arkanoid", { 0, 0, 0 }, 200);
	subtext->DrawText(Renderer, "Programmed by Avry Luy", { 0, 0, 0 }, 100);
	life_count->DrawText(Renderer, "Lives Left: " + toString(ball->get_life()), { 0, 0, 0, 0 }, 75);
	//Start gameLoop
	GameLoop();
	//exit code
	return 0;
}

void GameManager::Init_Level() {
	ball = new Ball(platform->get_plat_center(), platform->get_y() - 20);
	int nextblockid = 0;
	int currentblocktype = 0;
	//Block* blocks = new Block(blockColor[currentblocktype], blockHealth[currentblocktype]);
	

	for (int row = 0; row < 5; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			switch (row)
			{
			case 0:
				currentblocktype = 0;
				break;
			case 1:
				currentblocktype = 0;
				break;

			case 2:
				currentblocktype = 1;
				break;

			case 3:
				currentblocktype = 1;
				break;

			case 4:
				currentblocktype = 2;
				break;

			case 5:
				currentblocktype = 3;
				break;

			default:
				break;
			}
			int x = col * (720 / 10) - 10;
			int y = row * (720 / 10);
			//blocks->set_block_id(nextblockid);
			//blocks->set_x(j * (720 / 10) - 10);
			//blocks->set_w(70);
			//blocks->set_y(i * (720 / 10));
			//blocks->set_h((720 / 30) + 10);
			targets.push_back(Block(x, y, blockColor[currentblocktype], blockHealth[currentblocktype], nextblockid));
			nextblockid++;
			currentblocktype = (currentblocktype + 1) % NUMBLOCKTYPES;
		}
	}
}

void GameManager::GameLoop()
{
	//Load FPS manager objects
	FPSManager fps;
	fps.start_time();
	FPSManager cfps;
	//Keeps game loop running
	gameRunning = true;
	countedframes = 0;
	//actual game loop
	while (gameRunning)
	{
		cfps.start_time();
		//Averages frames to 60fps
		cfps.frame_avg(countedframes, fps.get_time());
		//SDL_Log("Countedframes %i", countedframes);
		//SDL_Log("GetTime %i", fps.get_time());
		// 
		//Handles keyboard events
		HandleEvents();
		ball->move(platform, platform->get_collider());
		platform->move_plat(scorescreen->get_x() - 150);

		for (Block& block: targets) { 
			if (block.isActive()) {
				if (ball->get_x() + ball->get_w() > block.get_x() && ball->get_x() < block.get_x() + block.get_w()
					&& ball->get_y() + ball->get_h() > block.get_y() && ball->get_y() < block.get_y() + block.get_h()) {
					//++score;
					SDL_Log("Hit Target :%i", block.get_block_id());
					SDL_Log("Block Health :%i", block.getHealth());
					
					block.damage(1);
					if (block.getHealth() < 1) {
						block.Destroy();
					}
					
					//ball->set_XDirection(5);
					ball->set_YDirection(5);
				}
			}
		}

		Render(Renderer);
		++countedframes;
		cfps.frame_limit(cfps.get_time());

	};
	//Quits game and destroys objects when game loop is exited
	Quit();
}

void GameManager::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || (event.key.keysym.sym == SDLK_ESCAPE))
		{
			gameRunning = false;
		
		}
		else if (event.key.keysym.sym == SDLK_r)
		{
			//Reset Game
			//ball->reset();
			ball->~Ball();
			targets.clear();
			Init_Level();
			//ball = new Ball(platform->get_plat_center(), platform->get_y() - 20);
		}
		platform->update(event);
		ball->update(event);
	}
}

void GameManager::Render(const TSharedPtr<renderer>& nRenderer)
{
	nRenderer->clear(88, 154, 186, 255);
	platform->draw_screen(nRenderer);
	scorescreen->draw_screen(nRenderer);
	text->renderText(nRenderer,(scorescreen->get_x() + 10), (scorescreen->get_y() + 30), 350, 90);
	subtext->renderText(nRenderer, (scorescreen->get_x() + 30), (scorescreen->get_y() + 130), 300, 50);
	//Ball Count Management

	life_count->DrawText(Renderer, "Lives Left: " + toString(ball->get_life()), { 0, 0, 0, 0 }, 75);


	life_count->renderText(nRenderer, (scorescreen->get_x() + 30), (scorescreen->get_y() + 170), 175, 50);
	if (ball) {
		ball->draw(nRenderer);
	}
	
	//block->draw(nRenderer);
	for (Block& block: targets )
	{
		if (block.isActive()) {
			block.draw(nRenderer);
		}
	}

	nRenderer->Present();
}

void GameManager::Quit()
{
	Renderer->~renderer();
	delete scorescreen;
	scorescreen = NULL;
	delete platform;
	platform = NULL;
	delete text;
	text = NULL;
	delete subtext;
	subtext = NULL;
	delete ball;
	ball = NULL;
	delete blocks;
	blocks = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}