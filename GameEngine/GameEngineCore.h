#pragma once

#include <GameEngineBase/GameEngineObjectBase.h>
#include "GameEngineManagerHelper.h"

// static class for Game
class GameEngineCore : public GameEngineObjectBase
{
protected:
	GameEngineCore();
	~GameEngineCore();

	GameEngineCore(const GameEngineCore& _other) = delete; 
	GameEngineCore(GameEngineCore&& _other) = delete; 

	GameEngineCore& operator=(const GameEngineCore& _other) = delete; 
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete;

public:
	template<typename UserGameType>
	static void Start()
	{
		initializeEngine();

		UserGameType NewUserGame;

		NewUserGame.Initialize();
		NewUserGame.LoadResource();

		MainCore = &NewUserGame;

		run();

		NewUserGame.Release();
		releaseEngine();
	}

public:
	virtual void Initialize() = 0;
	virtual void LoadResource() = 0;
	virtual void GameLoop() = 0;
	virtual void Release() = 0;


protected:
	static void initializeEngine();
	static void run();
	static void mainLoop();
	static void releaseEngine();

private:
	static GameEngineCore* MainCore;
};

