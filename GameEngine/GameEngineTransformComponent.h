#pragma once

#include "GameEngineComponent.h"

class GameEngineTransform;
class GameEngineTransformComponent : public GameEngineComponent
{
public:
	GameEngineTransformComponent(GameEngineActor* _actor); 
	~GameEngineTransformComponent();

	GameEngineTransformComponent(const GameEngineTransformComponent& _other) = delete; 
	GameEngineTransformComponent(GameEngineTransformComponent&& _other) = delete; 

	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _other) = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent&& _other) = delete;

private:
	GameEngineTransform* transform_;
};

