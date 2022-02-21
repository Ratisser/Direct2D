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

	virtual void Start() override;
	virtual void Update() override;

	void SetRenderingPipeline(const std::string& _name);
	void Render();
protected:
	GameEngineRenderingPipeline* pipe_;

	// GameEngineTransformComponent을(를) 통해 상속됨

};

