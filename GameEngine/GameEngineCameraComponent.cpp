#include "PreCompile.h"
#include "GameEngineCameraComponent.h"

GameEngineCameraComponent::GameEngineCameraComponent()
	: projectionMode_(ProjectionMode::Perspective)
	, FieldOfViewAngleY_(90.f)
	, nearZ_(0.1f)
	, farZ_(1000.f)
	, cameraSize_(1280.f, 720.f)
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

void GameEngineCameraComponent::Update(float _deltaTime)
{
	float4 eyePosition = GetWorldLocation();
	
	float4 eyeDirection = float4(0.0f, 0.0f, 1.0f, 0.0f) * transformData_.Wolrd;
	eyeDirection.Normalize3D();
	float4 upDirection = float4(0.0f, 1.0f, 0.0f, 0.0f) * transformData_.Wolrd;
	upDirection.Normalize3D();


	transformData_.View.ViewToLH(eyePosition, eyeDirection, upDirection);

	switch (projectionMode_)
	{
	case ProjectionMode::Perspective:
		transformData_.Projection.PerspectiveFovLH(FieldOfViewAngleY_, cameraSize_.x, cameraSize_.y, nearZ_, farZ_);
		break;
	case ProjectionMode::Orthographic:
		transformData_.Projection.OrthographicLH(cameraSize_.x, cameraSize_.y, nearZ_, farZ_);
		break;
	default:
		break;
	}
}
