#include "SDL.h"
#include "GameManager.h"


int main(int argc, char* args[])
{
	GameManager Game;
	Game.GameLoop();

	return 0;
}