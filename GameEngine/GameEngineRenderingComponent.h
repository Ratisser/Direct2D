#pragma once

#include "GameEngineTransformComponent.h"

class GameEngineRenderingPipeline;
class GameEngineRenderingComponent : public GameEngineTransformComponent
{
public:
	GameEngineRenderingComponent(); 
	~GameEngineRenderingComponent();

	GameEngineRenderingComponent(const GameEngineRenderingComponent& _other) = delete; 
	GameEngineRenderingComponent(GameEngineRenderingComponent&& _other) = delete; 

	GameEngineRenderingComponent& operator=(const GameEngineRenderingComponent& _other) = delete;
	GameEngineRenderingComponent& operator=(const GameEngineRenderingComponent&& _other) = delete;

	void SetRenderingPipeline(const std::string& _name);

protected:
	GameEngineRenderingPipeline* pipe_;
};

