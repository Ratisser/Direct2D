#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

class GameEngineActor;
class GameEngineComponent : public GameEngineObjectNameBase
{
public:
	GameEngineComponent(GameEngineActor* _actor); 
	~GameEngineComponent();

	GameEngineComponent(const GameEngineComponent& _other) = delete; 
	GameEngineComponent(GameEngineComponent&& _other) = delete; 

	GameEngineComponent& operator=(const GameEngineComponent& _other) = delete;
	GameEngineComponent& operator=(const GameEngineComponent&& _other) = delete;

	GameEngineActor* GetActor() const;

protected:
	GameEngineActor* actor_;
};

