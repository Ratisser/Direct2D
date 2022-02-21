#pragma once

enum class ProjectionMode
{
	Perspective,
	Orthographic
};

class GameEngineCameraComponent
{
public:
	GameEngineCameraComponent(); 
	~GameEngineCameraComponent();

	GameEngineCameraComponent(const GameEngineCameraComponent& _other) = delete; 
	GameEngineCameraComponent(GameEngineCameraComponent&& _other) = delete; 

	GameEngineCameraComponent& operator=(const GameEngineCameraComponent& _other) = delete;
	GameEngineCameraComponent& operator=(const GameEngineCameraComponent&& _other) = delete;

protected:
	ProjectionMode projectionMode_;
	float FieldOfViewAngleY_;
	float4 cameraSize_;
	float nearZ_;
	float farZ_;

};

