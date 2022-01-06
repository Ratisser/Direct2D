#include "PreCompile.h"

#include "Game.h"

int main()
{
	Game Game;
	Game.Initialize();
	Game.Run();
	Game.Release();
	return 0;
}