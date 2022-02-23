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

void GameEngineCameraComponent::Update()
{
	float4 eyePosition = transform_->GetWorldLocation();
	
	float4 eyeDirection = float4(0.0f, 0.0f, 1.0f, 0.0f) * transform_->GetTransformData().Wolrd;
	eyeDirection.Normalize3D();
	float4 upDirection = float4(0.0f, 1.0f, 0.0f, 0.0f) * transform_->GetTransformData().Wolrd;
	upDirection.Normalize3D();


	transform_->GetTransformData().View.ViewToLH(eyePosition, eyeDirection, upDirection);

	switch (projectionMode_)
	{
	case ProjectionMode::Perspective:
		transform_->GetTransformData().Projection.PerspectiveFovLH(FieldOfViewAngleY_, cameraSize_.x, cameraSize_.y, nearZ_, farZ_);
		break;
	case ProjectionMode::Orthographic:
		transform_->GetTransformData().Projection.OrthographicLH(cameraSize_.x, cameraSize_.y, nearZ_, farZ_);
		break;
	default:
		break;
	}
}
