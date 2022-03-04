#include "PreCompile.h"
#include "GameEngineTransform.h"

GameEngineTransform::GameEngineTransform()
	: scale_(1.0f, 1.0f, 1.0f, 0.0f)
	, rotation_(0.0f, 0.0f, 0.0f, 0.0f)
	, location_(0.0f, 0.0f, 0.0f, 1.0f)
	, parent_(nullptr)
{

}

GameEngineTransform::~GameEngineTransform()
{
}

void GameEngineTransform::SetScale(const float4& _scale)
{
	scale_ = _scale;
	scale_.z = 0.0f;
}

void GameEngineTransform::SetScale(float _scale)
{
	scale_ = { _scale, _scale, _scale, 0.0f };
}

void GameEngineTransform::SetScale(float _x, float _y, float _z)
{
	scale_.x = _x;
	scale_.y = _y;
	scale_.z = _z;
	scale_.w = 0.f;

}

void GameEngineTransform::MultiplyScale(float _scale)
{
	scale_.x *= _scale;
	scale_.y *= _scale;
	scale_.z *= _scale;
	scale_.w = 0.f;
}

void GameEngineTransform::MultiplyScale(float _x, float _y, float _z)
{
	scale_.x *= _x;
	scale_.y *= _y;
	scale_.z *= _z;
	scale_.w = 0.f;
}

void GameEngineTransform::SetRotation(const float4& _rotation)
{
	rotation_ = _rotation;
}

void GameEngineTransform::SetRotation(float _x, float _y, float _z)
{
	rotation_.x = _x;
	rotation_.y = _y;
	rotation_.z = _z;
	rotation_.w = 0.f;
}

void GameEngineTransform::AddRotation(float _x, float _y, float _z)
{
	rotation_.x += _x;
	rotation_.y += _y;
	rotation_.z += _z;
	rotation_.w = 0.f;
}

void GameEngineTransform::SetLocation(const float4& _location)
{
	location_ = _location;
}

void GameEngineTransform::SetLocation(float _x, float _y, float _z)
{
	location_.x = _x;
	location_.y = _y;
	location_.z = _z;
	location_.w = 1.0f;
}

void GameEngineTransform::AddLocation(float _x, float _y, float _z)
{
	location_.x += _x;
	location_.y += _y;
	location_.z += _z;
	location_.w = 1.0f;
}

float4 GameEngineTransform::GetScale()
{
	return scale_;
}

float4 GameEngineTransform::GetRotation()
{
	return rotation_;
}

float4 GameEngineTransform::GetLocation()
{
	return location_;
}

float4 GameEngineTransform::GetWorldLocation()
{
	return location_ * transformData_.Wolrd;
}

float4 GameEngineTransform::GetForward()
{
	return transformData_.Wolrd.vz.NormalizeReturn3D();
}

float4 GameEngineTransform::GetUp()
{
	return transformData_.Wolrd.vy.NormalizeReturn3D();
}

float4 GameEngineTransform::GetRight()
{
	return transformData_.Wolrd.vx.NormalizeReturn3D();
}

void GameEngineTransform::UpdateTransform()
{
	if (nullptr == parent_)
	{

		transformData_.Wolrd = float4x4::ScalingMatrixFromVector(scale_);
		transformData_.Wolrd = transformData_.Wolrd * float4x4::RotationMatrixFromVector(rotation_);
		transformData_.Wolrd = transformData_.Wolrd * float4x4::TranslationMatrixFromVector(location_);

		for (GameEngineTransform* child : children_)
		{
			child->UpdateTransformByParent();
		}
	}
}

void GameEngineTransform::UpdateTransformByParent()
{
	transformData_.Wolrd = float4x4::ScalingMatrixFromVector(scale_);
	transformData_.Wolrd = transformData_.Wolrd * float4x4::RotationMatrixFromVector(rotation_);
	transformData_.Wolrd = transformData_.Wolrd * float4x4::TranslationMatrixFromVector(location_);

	transformData_.Wolrd = transformData_.Wolrd * parent_->transformData_.Wolrd;

	// TODO : 스택 오버플로우 위험이 있음.
	for (GameEngineTransform* child : children_)
	{
		child->UpdateTransformByParent();
	}
}

TransformData& GameEngineTransform::GetTransformData()
{
	return transformData_;
}

void GameEngineTransform::SetParent(GameEngineTransform* _parent)
{
	if (nullptr != parent_)
	{
		parent_->RemoveChild(this);
	}

	parent_ = _parent;
	parent_->AddChild(this);
}

void GameEngineTransform::UnsetParent()
{
	if (nullptr != parent_)
	{
		parent_->RemoveChild(this);
		parent_ = nullptr;
	}
}

void GameEngineTransform::AddChild(GameEngineTransform* _child)
{
	children_.push_back(_child);
}

void GameEngineTransform::RemoveChild(GameEngineTransform* _child)
{
	children_.remove(_child);
}

std::list<GameEngineTransform*>& GameEngineTransform::GetChildren()
{
	return children_;
}