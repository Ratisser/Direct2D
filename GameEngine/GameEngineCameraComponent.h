#pragma once

#include "GameEngineTransformComponent.h"

enum class ProjectionMode
{
	Perspective,
	Orthographic
};

class GameEngineCameraComponent : GameEngineTransformComponent
{
public:
	GameEngineCameraComponent(); 
	~GameEngineCameraComponent();

	GameEngineCameraComponent(const GameEngineCameraComponent& _other) = delete; 
	GameEngineCameraComponent(GameEngineCameraComponent&& _other) = delete; 

	GameEngineCameraComponent& operator=(const GameEngineCameraComponent& _other) = delete;
	GameEngineCameraComponent& operator=(const GameEngineCameraComponent&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void SetProjectionMode(ProjectionMode _mode);

protected:
	ProjectionMode projectionMode_;
	float FieldOfViewAngleY_;
	float4 cameraSize_;
	float nearZ_;
	float farZ_;

};

