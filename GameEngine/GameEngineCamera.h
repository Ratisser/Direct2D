#pragma once

#include <GameEngine/GameEngineCameraComponent.h>
#include "GameEngineActor.h"

class GameEngineCameraComponent;
class GameEngineLevel;
class GameEngineCamera final : public GameEngineActor
{
	friend GameEngineLevel;
public:
	GameEngineCamera();
	~GameEngineCamera();

	GameEngineCamera(const GameEngineCamera& _other) = delete;
	GameEngineCamera(GameEngineCamera&& _other) = delete;

	GameEngineCamera& operator=(const GameEngineCamera& _other) = delete;
	GameEngineCamera& operator=(const GameEngineCamera&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	GameEngineCameraComponent* GetCamera();

private:
	float4x4 getViewMatrix();
	float4x4 getProjectionMatrix();
private:
	GameEngineCameraComponent* camera_;
};

