#include "PreCompile.h"
#include "GameEngineCamera.h"
#include "GameEngineCameraComponent.h"

GameEngineCamera::GameEngineCamera()
	: camera_(nullptr)
{

}

GameEngineCamera::~GameEngineCamera()
{

}

void GameEngineCamera::Start()
{
	camera_ = CreateTransformComponent<GameEngineCameraComponent>(GetTransform());
	//camera->SetProjectionMode(ProjectionMode::Orthographic);
}

void GameEngineCamera::Update(float _deltaTime)
{
}

GameEngineCameraComponent* GameEngineCamera::GetCamera()
{
	return camera_;
}

float4x4 GameEngineCamera::getViewMatrix()
{
	return camera_->GetTransformData().View;
}

float4x4 GameEngineCamera::getProjectionMatrix()
{
	return camera_->GetTransformData().Projection;
}
