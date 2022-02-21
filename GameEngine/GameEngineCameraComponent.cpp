#include "PreCompile.h"
#include "GameEngineCameraComponent.h"

GameEngineCameraComponent::GameEngineCameraComponent()
	: projectionMode_(ProjectionMode::Perspective)
	, FieldOfViewAngleY_(90.f)
	, nearZ_(0.001f)
	, farZ_(1000.f)
{

}

GameEngineCameraComponent::~GameEngineCameraComponent()
{

}

void GameEngineCameraComponent::SetProjectionMode(ProjectionMode _mode)
{
	projectionMode_ = _mode;
}

void GameEngineCameraComponent::Start()
{
}

void GameEngineCameraComponent::Update()
{
	
}
