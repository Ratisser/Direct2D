#pragma once

#include <GameEngineBase/GameEngineObjectBase.h>

class GameEngineCore : GameEngineObjectBase
{
public:
	GameEngineCore(); 
	~GameEngineCore();

	GameEngineCore(const GameEngineCore& _other) = delete; 
	GameEngineCore(GameEngineCore&& _other) = delete; 

	GameEngineCore& operator=(const GameEngineCore& _other) = delete;
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete;

	void Initialize();
	void Run();
	void Release();

protected:
	virtual void loadResources() = 0;
	virtual void release() = 0;
	
};

