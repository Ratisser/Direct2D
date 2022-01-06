#include "PreCompile.h"
#include "Game.h"



Game::Game()
{

}

Game::~Game()
{

}

void Game::loadResources()
{
	fs::path currentPath = fs::current_path();
	while (0 != currentPath.filename().compare("Direct2D"))
	{
		currentPath = currentPath.parent_path();
	}

	currentPath /= "Resource";
	
	fs::path soundPath = currentPath / "sound";
	
	for (auto& path : fs::recursive_directory_iterator(soundPath))
	{
		GameEngineSoundManager::GetInstance().CreateSound(path.path().filename().string(), path.path().string());
	}
}

void Game::release()
{
}
