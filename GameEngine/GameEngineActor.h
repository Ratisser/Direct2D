#pragma once

#include <GameEngineBase/GameEngineObjectBase.h>

#include "GameEngineTransform.h"

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
	virtual void Start() = 0;
	virtual void Update(float _deltaTime) = 0;

	GameEngineLevel* GetLevel();
	void SetLevel(GameEngineLevel* _level);

protected:
	GameEngineLevel* level_;

private:
	std::unique_ptr<GameEngineTransform> transform_;
};

