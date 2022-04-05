#include "PreCompile.h"
#include "GameEngineTransformComponent.h"

GameEngineTransformComponent::GameEngineTransformComponent()
	: scale_(1.0f, 1.0f, 1.0f, 0.0f)
	, rotation_(0.0f, 0.0f, 0.0f, 0.0f)
	, location_(0.0f, 0.0f, 0.0f, 1.0f)
	, parent_(nullptr)
{

}

GameEngineTransformComponent::~GameEngineTransformComponent()
{
}

void GameEngineTransformComponent::SetScale(const float4& _scale)
{
	scale_ = _scale;
	scale_.z = 0.0f;
}

void GameEngineTransformComponent::SetScale(float _scale)
{
	scale_ = { _scale, _scale, _scale, 0.0f };
}

void GameEngineTransformComponent::SetScale(float _x, float _y, float _z)
{
	scale_.x = _x;
	scale_.y = _y;
	scale_.z = _z;
	scale_.w = 0.f;
}

void GameEngineTransformComponent::AddScale(float _scale)
{
	scale_.x += _scale;
	scale_.y += _scale;
	scale_.z += _scale;
	scale_.w = 0.f;
}

void GameEngineTransformComponent::AddScale(float _x, float _y, float _z)
{
	scale_.x += _x;
	scale_.y += _y;
	scale_.z += _z;
	scale_.w = 0.f;
}

void GameEngineTransformComponent::MultiplyScale(float _scale)
{
	scale_.x *= _scale;
	scale_.y *= _scale;
	scale_.z *= _scale;
	scale_.w = 0.f;
}

void GameEngineTransformComponent::MultiplyScale(float _x, float _y, float _z)
{
	scale_.x *= _x;
	scale_.y *= _y;
	scale_.z *= _z;
	scale_.w = 0.f;
}

void GameEngineTransformComponent::SetRotation(const float4& _rotation)
{
	rotation_ = _rotation;
}

void GameEngineTransformComponent::SetRotation(float _x, float _y, float _z)
{
	rotation_.x = _x;
	rotation_.y = _y;
	rotation_.z = _z;
	rotation_.w = 0.f;
}

void GameEngineTransformComponent::AddRotation(float _x, float _y, float _z)
{
	rotation_.x += _x;
	rotation_.y += _y;
	rotation_.z += _z;
	rotation_.w = 0.f;
}

void GameEngineTransformComponent::SetLocation(const float4& _location)
{
	location_ = _location;
}

void GameEngineTransformComponent::SetLocation(float _x, float _y, float _z)
{
	location_.x = _x;
	location_.y = _y;
	location_.z = _z;
	location_.w = 1.0f;
}

void GameEngineTransformComponent::AddLocation(float _x, float _y, float _z)
{
	location_.x += _x;
	location_.y += _y;
	location_.z += _z;
	location_.w = 1.0f;
}

void GameEngineTransformComponent::AddLocation(const float4& _location)
{
	location_ += _location;
}

float4 GameEngineTransformComponent::GetScale() const
{
	return scale_;
}

float4 GameEngineTransformComponent::GetRotation() const
{
	return rotation_;
}

float4 GameEngineTransformComponent::GetLocation() const
{
	return location_;
}

float4 GameEngineTransformComponent::GetWorldLocation() const
{
	return transformData_.Wolrd.vw;
}

float4 GameEngineTransformComponent::GetWorldScale() const
{
	if (parent_)
	{
		return scale_ * parent_->scale_;
	}
	return scale_;
}

float4 GameEngineTransformComponent::GetForward() const
{
	return transformData_.Wolrd.vz.NormalizeReturn3D();
}

float4 GameEngineTransformComponent::GetUp() const
{
	return transformData_.Wolrd.vy.NormalizeReturn3D();
}

float4 GameEngineTransformComponent::GetRight() const
{
	return transformData_.Wolrd.vx.NormalizeReturn3D();
}

void GameEngineTransformComponent::UpdateTransform()
{
	if (nullptr == parent_)
	{

		transformData_.Wolrd = float4x4::ScalingMatrixFromVector(scale_);
		transformData_.Wolrd = transformData_.Wolrd * float4x4::RotationMatrixFromVector(rotation_);
		transformData_.Wolrd = transformData_.Wolrd * float4x4::TranslationMatrixFromVector(location_);

		for (GameEngineTransformComponent* child : children_)
		{
			child->UpdateTransformByParent();
			child->Update(0.0f);
		}
	}
}

void GameEngineTransformComponent::UpdateTransformByParent()
{
	transformData_.Wolrd = float4x4::ScalingMatrixFromVector(scale_);
	transformData_.Wolrd = transformData_.Wolrd * float4x4::RotationMatrixFromVector(rotation_);
	transformData_.Wolrd = transformData_.Wolrd * float4x4::TranslationMatrixFromVector(location_);

	transformData_.Wolrd = transformData_.Wolrd * parent_->transformData_.Wolrd;

	// TODO : 스택 오버플로우 위험이 있음.
	for (GameEngineTransformComponent* child : children_)
	{
		child->UpdateTransformByParent();
		child->Update(0.0f);
	}
}

void GameEngineTransformComponent::ReleaseReady()
{
	UnsetParent();
	for (GameEngineTransformComponent* child : children_)
	{
		child->ReleaseReady();
		child->Death();
	}
}

TransformData& GameEngineTransformComponent::GetTransformData()
{
	return transformData_;
}

void GameEngineTransformComponent::SetParent(GameEngineTransformComponent* _parent)
{
	if (nullptr != parent_)
	{
		parent_->RemoveChild(this);
	}

	parent_ = _parent;
	parent_->AddChild(this);
}

void GameEngineTransformComponent::UnsetParent()
{
	if (nullptr != parent_)
	{
		parent_->RemoveChild(this);
		parent_ = nullptr;
	}
}

void GameEngineTransformComponent::AddChild(GameEngineTransformComponent* _child)
{
	children_.push_back(_child);
}

void GameEngineTransformComponent::RemoveChild(GameEngineTransformComponent* _child)
{
	children_.remove(_child);
}

std::list<GameEngineTransformComponent*>& GameEngineTransformComponent::GetChildren()
{
	return children_;
}

void GameEngineTransformComponent::Start()
{
	UpdateTransform();
}

void GameEngineTransformComponent::Update(float _deltaTime)
{
}
