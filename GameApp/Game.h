#pragma once

#include <GameEngine/GameEngineCore.h>

class Game final: public GameEngineCore
{
public:
	Game(); 
	~Game();

	Game(const Game& _other) = delete; 
	Game(Game&& _other) = delete; 

	Game& operator=(const Game& _other) = delete;
	Game& operator=(const Game&& _other) = delete;


private:
	virtual void loadResources() override;
	virtual void release() override;
};

