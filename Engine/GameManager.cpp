#include "Engine\GameManager.h"


GameManager::GameManager()
{
	//Construct objects for game
	scorescreen = new ScoreScreen();
	text = new TextTextures("Title");
	subtext = new TextTextures("Sub-title");
	instructions = new TextTextures("Instructions");
	instructions2 = new TextTextures("Instructions2");
	life_count = new TextTextures("Life Counter");
	score_count = new TextTextures("Score");
	game_over = new TextTextures("GameOver");
	game_win = new TextTextures("GameWin");
	paddle_ball = new LTexture();
	block_texture = new LTexture();
	scoreScreenTexture = new LTexture();
	mainScreenTexture = new LTexture();
	ball = nullptr;
	blocks = nullptr;
	platform = nullptr;
	bricksFile = "";
	paddleBallFile = "";
	backgroundFile = "";
	
	gameState = true;

	mainScreenRect = { 0, 0, 0, 0 };

	for (int i = 0; i < 2; i++)
	{
		ScreenRects[i] = { 0, 0, 0, 0 };
	}

	for (int i = 0; i < 12; i++)
	{
		Tblock[i] = { 0, 0, 0, 0 };
	}

	for (int i = 0; i < 2; i++)
	{
		pballarray[i] = { 0, 0, 0, 0 };
	}
	
	for (int i = 0; i < 6; i++)
	{
		audiofile[i] = "";
	}


}

GameManager::~GameManager()
{
	Renderer = NULL;
}

int GameManager::Init() {
	//SDL_Log("Running GameManager::Init()\n");
	//Initialize Game window, renderer, and text
	if (SDL_Init(SDL_INIT_TIMER & SDL_INIT_VIDEO) == 0)
	{
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		TTF_Init();
	};
	//Create Renderer
	Renderer = renderer::Construct(window);
	main_surf = SDL_GetWindowSurface(window);
	text->DrawText(Renderer,"Arkanoid", { 229, 178, 245 }, 200);
	subtext->DrawText(Renderer, "Programmed by Avry Luy", { 229, 178, 245 }, 100);
	instructions->DrawText(Renderer, instructionsText, { 229, 178, 245 }, 85);
	instructions2->DrawText(Renderer, instructions2Text, { 229, 178, 245 }, 75);

	game_over->DrawText(Renderer, "Game Over :(", { 229, 178, 245 }, 300);
	game_win->DrawText(Renderer, "Congrats, you won! Score:" + toString(gameScore), {229, 178, 245}, 300);
	loadAssets();
	Init_Level();
	//Start gameLoop
	GameLoop();
	//exit code
	return 0;
}

void GameManager::Init_Level() {
	//SDL_Log("Running GameManager::Init_Level()\n");
	audiomanager.play_sound(&musIntro);
	platform = new Platform(pballarray[1].w, pballarray[1].h, 3.6f);
	ball = new Ball(pballarray[0].w, pballarray[0].h, 2.1f);


	int nextblockid = 0;
	int currentblocktype = 0;
	int wholeTexture = 0;
	gameScore = 0;
	//Block* blocks = new Block(blockColor[currentblocktype], blockHealth[currentblocktype]);


	for (int row = 0; row < 5; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			switch (row)
			{
			case 0:
				currentblocktype = 0;
				wholeTexture = 0;
				break;
			case 1:
				currentblocktype = 0;
				wholeTexture = 0;

				break;

			case 2:
				currentblocktype = 1;
				wholeTexture = 3;

				break;

			case 3:
				currentblocktype = 1;
				wholeTexture = 3;

				break;

			case 4:
				currentblocktype = 2;
				wholeTexture = 6;

				break;

			case 5:
				currentblocktype = 3;
				wholeTexture = 9;

				break;

			default:
				break;
			}
			int x = col * (Tblock[wholeTexture].w + 40);
			int y = row * (Tblock[wholeTexture].h + 40);
			//targets.push_back(Block(x, y, blockColor[currentblocktype], blockHealth[currentblocktype], nextblockid, blockScores[currentblocktype]));
			targets.push_back(Block(x, y, Tblock[wholeTexture].w, Tblock[wholeTexture].h, blockHealth[currentblocktype], nextblockid, blockScores[currentblocktype], wholeTexture, 2.0f));

			//printf("Col %i, Row %i: BlockID = %i\n", x, y, targets[nextblockid].get_block_id());
			nextblockid++;
			currentblocktype = (currentblocktype + 1) % NUMBLOCKTYPES;
		}
	}
	remainingBlocks.reserve(targets.size());
	//SDL_Log("Targets size: %I64i\n", targets.size());
}

int GameManager::loadAssets() {
	bool success = false;

	//Set Audio File Paths
	
	audiofile[0] = "Audio/music_intro.wav";
	audiofile[1] = "Audio/block_hit.wav";
	audiofile[2] = "Audio/block_destroy.wav";
	audiofile[3] = "Audio/ball_death.wav";
	audiofile[4] = "Audio/game_over.wav";
	audiofile[5] = "Audio/game_win.wav";
	audiofile[6] = "Audio/col_hit.wav";

	//Set Art Asset Paths
	paddleBallFile = "Assets/paddles_and_balls.png";
	bricksFile = "Assets/bricks.png";
	backgroundFile = "Assets/Background_Tiles.png";

	//Load Audio Files
	if (!audiomanager.load_sound(audiofile[0], musIntro))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[0]);
		return -1;
	}
	if (!audiomanager.load_sound(audiofile[1], blockHitSnd))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[1]);
		return -1;
	}
	if (!audiomanager.load_sound(audiofile[2], blockDestroySnd))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[2]);
		return -1;
	}
	if (!audiomanager.load_sound(audiofile[3], ballDeathSnd))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[3]);
		return -1;
	}
	if (!audiomanager.load_sound(audiofile[4], gameOverSnd))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[4]);
		return -1;
	}
	if (!audiomanager.load_sound(audiofile[5], gameCompleteSnd))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[5]);
		return -1;
	}

	if (!audiomanager.load_sound(audiofile[6], colHitSnd))
	{
		SDL_LogError(5, "Failed to load sound: %s\n", audiofile[6]);
		return -1;
	}


	// Load Art Files
	if (!paddle_ball->loadImage(Renderer, paddleBallFile))
	{
		SDL_LogError(6, "Couldn't load image into Texture: %s\n", paddleBallFile);
		exit(EXIT_FAILURE);
	}

	if (!block_texture->loadImage(Renderer, bricksFile))
	{
		SDL_LogError(6, "Couldn't load image into Texture: %s\n", bricksFile);
		exit(EXIT_FAILURE);
	}

	if (!scoreScreenTexture->loadImage(Renderer, backgroundFile))
	{
		SDL_LogError(6, "Couldn't load image into Texture: %s\n", backgroundFile);
		exit(EXIT_FAILURE);
	}

	if (!mainScreenTexture->loadImage(Renderer, backgroundFile))
	{
		SDL_LogError(6, "Couldn't load image into Texture: %s\n", "..\\Arkanoid\\Assets\\mainbackground.png");
		exit(EXIT_FAILURE);
	}

	// Focus asset coordinates for each object
		
	//Main Screen
	ScreenRects[0].x = 160;
	ScreenRects[0].y = 0;
	ScreenRects[0].w = 32;
	ScreenRects[0].h = 96;


	//ScoreScreen
	ScreenRects[1].x = 192;
	ScreenRects[1].y = 0;
	ScreenRects[1].w = 32;
	ScreenRects[1].h = 96;

	//ball
	pballarray[0].x = 176;
	pballarray[0].y = 0;
	pballarray[0].w = 16;
	pballarray[0].h = 16;

	//paddle
	pballarray[1].x = 64;
	pballarray[1].y = 7;
	pballarray[1].w = 32;
	pballarray[1].h = 9;

	// Whole Blue Block
	Tblock[0].x = 112;
	Tblock[0].y = 0;
	Tblock[0].w = 32;
	Tblock[0].h = 16;

	//Slightly Damaged Blue Block
	Tblock[1].x = 144;
	Tblock[1].y = 0;
	Tblock[1].w = 32;
	Tblock[1].h = 16;

	//Heavily Damaged Blue Block
	Tblock[2].x = 176;
	Tblock[2].y = 0;
	Tblock[2].w = 32;
	Tblock[2].h = 16;

	// Whole Green Block
	Tblock[3].x = 111;
	Tblock[3].y = 16;
	Tblock[3].w = 32;
	Tblock[3].h = 16;

	//Slightly Damaged Green Block
	Tblock[4].x = 144;
	Tblock[4].y = 16;
	Tblock[4].w = 64;
	Tblock[4].h = 32;

	//Heavily Damaged Green Block
	Tblock[5].x = 176;
	Tblock[5].y = 16;
	Tblock[5].w = 64;
	Tblock[5].h = 32;

	// Whole Yellow Block
	Tblock[6].x = 111;
	Tblock[6].y = 32;
	Tblock[6].w = 32;
	Tblock[6].h = 16;

	//Slightly Damaged Yellow Block
	Tblock[7].x = 144;
	Tblock[7].y = 32;
	Tblock[7].w = 32;
	Tblock[7].h = 32;

	//Heavily Damaged Yellow Block
	Tblock[8].x = 176;
	Tblock[8].y = 32;
	Tblock[8].w = 32;
	Tblock[8].h = 16;

	// Whole Orange Block
	Tblock[9].x = 111;
	Tblock[9].y = 48;
	Tblock[9].w = 32;
	Tblock[9].h = 16;

	//Slightly Damaged Orange Block
	Tblock[10].x = 144;
	Tblock[10].y = 48;
	Tblock[10].w = 32;
	Tblock[10].h = 16;

	//Heavily Damaged Orange Block
	Tblock[11].x = 176;
	Tblock[11].y = 48;
	Tblock[11].w = 32;
	Tblock[11].h = 16;

	if (block_texture && &colHitSnd && &blockDestroySnd
		&& &sound1 && paddle_ball && scoreScreenTexture)
	{
		success = true;
	}

	if (success)
	{
		return 0;
	}
	else {
		return -1;
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
	bool gameOver = false;

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
		bool ballExist = true;
		HandleEvents();
		if (ball->get_life() > 0)
		{
			ball->move(platform, platform->get_collider());
			if (ball->ballCollideWall())
			{
				audiomanager.play_sound(&colHitSnd);
				ball->setBallBool();
			}
			platform->move_plat(scorescreen->get_x());


			for (Block& block : targets) {
				if (block.isActive() && !block.getCollision()) {
					if (ball->get_x() + ball->get_w() > block.get_x() && ball->get_x() < block.get_x() + block.get_w()
						&& ball->get_y() + ball->get_h() > block.get_y() && ball->get_y() < block.get_y() + block.get_h()) {

						// Mark the block as hit for this frame
						block.setCollision(true);

						// Apply damage and play sound
						block.damage(1);
						audiomanager.play_sound(&blockHitSnd);

						if (block.getHealth() < 1) {
							gameScore += block.get_block_score();
							block.Destroy();
							audiomanager.play_sound(&blockDestroySnd);
							//SDL_Log("Targets size: %I64i\n", targets.size());
							
						}
						else
						{
							remainingBlocks.push_back(block);
						}

						// Move the ball out of collision to avoid subsequent hits
						if (ball->get_y() + ball->get_h() > block.get_y() && ball->get_y() < block.get_y()) {
							ball->set_YDirection(-5);
							ball->set_y(block.get_y() - ball->get_h() - 1);
						}
						else {
							ball->set_YDirection(5);
							ball->set_y(block.get_y() + block.get_h() + 1);
						}

						// Log debug info
						//SDL_Log("Hit Target :%i\n", block.get_block_id());
						//SDL_Log("Block Health :%i\n", block.getHealth());
					}
					else {
						remainingBlocks.push_back(block);
					}
				}
				else
				{
					remainingBlocks.push_back(block);
				}
			}

			targets = std::move(remainingBlocks);
			// After checking all blocks, reset their hit status
			for (Block& block : targets) {
				block.setCollision(false);
			}
			if (ball->lifeChanged() == true && ballExist == true)
			{

				//SDL_Log("lifeChanged: %s | BlockExist: %s\n", ball->lifeChanged() ? "true":"false", ballExist ? "true":"false");
				ballExist = false;

				audiomanager.play_sound(&ballDeathSnd);
				
			}
		}

		else if (ball->get_life() == 0 && !gameOver)

		{
			audiomanager.play_sound(&gameOverSnd);
			gameOver = true;
			// Add lose condition
		}
		if (targets.empty())
		{
			// Win condition
			gameState = false;
			
			//SDL_Log("Targets Empty condition\n");
			ball->set_XDirection(0);
			ball->set_YDirection(0);
			if (!gameOver)
			{
				audiomanager.play_sound(&gameCompleteSnd);
				ball->winCon();
				gameOver = true;
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
	//printf("Init Game State: %s\n", gameState ? "true" : "false");
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (gameState)
		{
			platform->update(event, gameState);
			ball->update(event);
		}
		
		if (event.type == SDL_QUIT || (event.key.keysym.sym == SDLK_ESCAPE))
		{
			gameRunning = false;
		
		}
		else if (event.key.keysym.sym == SDLK_r)
		{
			//Reset Game
			//ball->reset();
			targets.clear();
			Init_Level();
		}
	}
}

void GameManager::Render(const TSharedPtr<renderer>& nRenderer)
{
	nRenderer->clear(255, 255, 255, 255);
	nRenderer->renderBackground(mainScreenTexture->getTexture(), &ScreenRects[0],
		SCREEN_WIDTH - (scorescreen->get_w()), SCREEN_HEIGHT);
	platform->renderPlat(nRenderer, platform->get_x(), platform->get_y(),
		pballarray[1].w, pballarray[1].h, paddle_ball->getTexture(),
		&pballarray[1], 3.6f);
	//platform->draw_screen(nRenderer);
	//scorescreen->draw_screen(nRenderer);
	scorescreen->render(nRenderer, scorescreen->get_x(), scorescreen->get_y(),
		scorescreen->get_w(), scorescreen->get_y(), scoreScreenTexture->getTexture(), &ScreenRects[1]);
	text->renderText(nRenderer,(scorescreen->get_x() + 5), (scorescreen->get_y() + 5),
		350, 90);
	subtext->renderText(nRenderer, (scorescreen->get_x() + 25), (scorescreen->get_y() + 80),
		300, 50);
	instructions->renderText(nRenderer, (scorescreen->get_x() + 5), (scorescreen->get_h() - 100),
		scorescreen->get_w() - 10, 50);
	instructions2->renderText(nRenderer, (scorescreen->get_x() + 5), (scorescreen->get_h() - 50),
		scorescreen->get_w() - 10, 50);

	//Ball Count Management

	life_count->DrawText(Renderer, "Lives Left: " + toString(ball->get_life()),
		{ 229, 178, 245, 0 }, 150);
	score_count->DrawText(Renderer, "Score: " + toString(gameScore),
		{ 229, 178, 245, 0 }, 150);


	life_count->renderText(nRenderer, (scorescreen->get_x() + 25),
		(scorescreen->get_y() + 200), 300, 50);
	score_count->renderText(nRenderer, (scorescreen->get_x() + 25),
		(scorescreen->get_y() + 310), 300, 60);
	if (ball) {
		//ball->draw(nRenderer);
		ball->renderBall(nRenderer, ball->get_x(), ball->get_y(),
			pballarray[0].w, pballarray[0].h, paddle_ball->getTexture(),
			&pballarray[0], 1.5f);
	}
	//game_win->DrawText(nRenderer, game_win->get_text().c_str(), { 229, 178, 245 }, 400);
	//block->draw(nRenderer);
	for (Block& block: targets )
	{
		if (block.isActive()) {
			//If block hit once
			if (block.getHealth() == (block.getStartHealth() - 1))
			{
				block.render(nRenderer, block.get_x(), block.get_y(), block.get_w(), block.get_h(), block_texture->getTexture(), &Tblock[block.get_texture_id()+1]);

			}
			//If block hit more than once
			else if (block.getHealth() > 0 && block.getHealth() < block.getStartHealth() - 1)
			{
				block.render(nRenderer, block.get_x(), block.get_y(), block.get_w(), block.get_h(), block_texture->getTexture(), &Tblock[block.get_texture_id()+2]);

			}
			else
			{
				block.render(nRenderer, block.get_x(), block.get_y(), block.get_w(), block.get_h(), block_texture->getTexture(), &Tblock[block.get_texture_id()]);
			}
			
		}
	}

	if (ball && ball->get_life() == 0)
	{
		nRenderer->clear(255, 255, 255, 255);
		nRenderer->renderBackground(mainScreenTexture->getTexture(), &ScreenRects[0],
			SCREEN_WIDTH - (scorescreen->get_w()), SCREEN_HEIGHT);
		scorescreen->render(nRenderer, scorescreen->get_x(), scorescreen->get_y(),
			scorescreen->get_w(), scorescreen->get_y(), scoreScreenTexture->getTexture(), &ScreenRects[1]);
		text->renderText(nRenderer, (scorescreen->get_x() + 5), (scorescreen->get_y() + 5),
			350, 90);
		subtext->renderText(nRenderer, (scorescreen->get_x() + 25), (scorescreen->get_y() + 80),
			300, 50);
		instructions->renderText(nRenderer, (scorescreen->get_x() + 5), (scorescreen->get_h() - 100),
			scorescreen->get_w() - 10, 50);
		instructions2->renderText(nRenderer, (scorescreen->get_x() + 5), (scorescreen->get_h() - 50),
			scorescreen->get_w() - 10, 50);

		//Ball Count Management

		life_count->DrawText(Renderer, "Lives Left: " + toString(ball->get_life()),
			{ 229, 178, 245, 0 }, 150);
		score_count->DrawText(Renderer, "Score: " + toString(gameScore),
			{ 229, 178, 245, 0 }, 150);


		life_count->renderText(nRenderer, (scorescreen->get_x() + 25),
			(scorescreen->get_y() + 200), 300, 50);
		score_count->renderText(nRenderer, (scorescreen->get_x() + 25),
			(scorescreen->get_y() + 310), 300, 60);

		game_over->renderText(nRenderer, (int)((SCREEN_WIDTH * .5) - 400), (int)(SCREEN_HEIGHT * .5), 400, 100);
	}
	else if (targets.empty())
	{
		// Win con
		game_win->DrawText(nRenderer,"Congrats, you won! Score:" + toString(gameScore), { 229, 178, 245, 0 }, 350);
		game_win->renderText(nRenderer, (int)(SCREEN_WIDTH * .3)- 150, (int)(SCREEN_HEIGHT * .3), 400, 100);
	
	}
	nRenderer->Present();
}

void GameManager::Quit()
{
	Renderer->~renderer();
	Renderer = NULL;
	audiomanager.~AudioManager();
	//gameObjects
	delete platform;
	platform = NULL;
	delete ball;
	ball = NULL;
	delete blocks;
	blocks = NULL;
	remainingBlocks.clear();
	delete scorescreen;
	scorescreen = NULL;
	delete text;
	text = NULL;
	delete subtext;
	subtext = NULL;
	delete life_count;
	life_count = NULL;
	delete score_count;
	score_count = NULL;
	delete game_over;
	game_over = NULL;
	delete game_win;
	game_win = NULL;

	//Textures
	main_surf = nullptr;
	mainScreenTexture->~LTexture();
	mainScreenTexture = nullptr;
	scoreScreenTexture->~LTexture();
	scoreScreenTexture = nullptr;
	paddle_ball->~LTexture();
	paddle_ball = nullptr;
	block_texture->~LTexture();
	block_texture = nullptr;



	SDL_DestroyWindow(window);
	window = NULL;
	
	PaError err = Pa_Terminate();
	if (err != paNoError)
	{
		exit(EXIT_FAILURE);
	}

	SDL_Quit();
}