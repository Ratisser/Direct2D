#pragma once

#include <GameEngineBase/GameEngineObjectBase.h>

class GameEngineLevel;
class GameEngineActor : public GameEngineObjectBase
{
	friend GameEngineLevel;
public:
	GameEngineActor(); 
	~GameEngineActor();

	GameEngineActor(const GameEngineActor& _other) = delete; 
	GameEngineActor(GameEngineActor&& _other) = delete; 

	GameEngineActor& operator=(const GameEngineActor& _other) = delete;
	GameEngineActor& operator=(const GameEngineActor&& _other) = delete;

public:
	virtual void Update(float _deltaTime) = 0;

protected:
	GameEngineLevel* GetLevel() const;
	void SetLevel(GameEngineLevel* _level);

protected:
	GameEngineLevel* level_;
};

