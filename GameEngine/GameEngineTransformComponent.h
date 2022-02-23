#pragma once

#include "GameEngineComponent.h"
#include "GameEngineTransform.h"

class GameEngineTransform;
class GameEngineTransformComponent : public GameEngineComponent
{
public:
	GameEngineTransformComponent(); 
	virtual ~GameEngineTransformComponent();

	GameEngineTransformComponent(const GameEngineTransformComponent& _other) = delete; 
	GameEngineTransformComponent(GameEngineTransformComponent&& _other) = delete; 

	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _other) = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent&& _other) = delete;

	GameEngineTransform* GetTransform() { return transform_; }

protected:
	GameEngineTransform* transform_;
};
