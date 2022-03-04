#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

class GameEngineActor;
class GameEngineLevel;
class GameEngineComponent : public GameEngineObjectNameBase
{
public:
	GameEngineComponent(); 
	virtual ~GameEngineComponent();

	GameEngineComponent(const GameEngineComponent& _other) = delete; 
	GameEngineComponent(GameEngineComponent&& _other) = delete; 

	GameEngineComponent& operator=(const GameEngineComponent& _other) = delete;
	GameEngineComponent& operator=(const GameEngineComponent&& _other) = delete;

public:
	virtual void Start() = 0;
	virtual void Update(float _deltaTime) = 0;

public:
	void InitComponent(GameEngineActor* _actor);

	GameEngineActor* GetActor() const;
	GameEngineLevel* GetLevel() const;

protected:
	GameEngineActor* actor_;
	GameEngineLevel* level_;
};

