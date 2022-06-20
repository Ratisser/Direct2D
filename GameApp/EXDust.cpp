#include "PreCompile.h"
#include "EXDust.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>
#include "EXDust.h"

EXDust::EXDust()
	: renderer_(nullptr)
	, childTransform_(nullptr)
	, bLeft_(false)
{

}

EXDust::~EXDust()
{

}

void EXDust::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationY(20.f);
	childTransform_->SetLocationZ(0.0001f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("SSDustA", 0.0416f, false);
	renderer_->SetLocationZ(0.001f);
	renderer_->SetPivot(eImagePivot::CENTER);

	renderer_->ChangeAnimation("SSDustA");
}

void EXDust::Update(float _deltaTime)
{
	if (bLeft_)
	{
		renderer_->SetFlip(true, false);
	}
	else
	{
		renderer_->SetFlip(false, false);
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}

void EXDust::Initialize(bool _bLeft, float _rotZAxis)
{
	bLeft_ = _bLeft;
	childTransform_->AddRotation(0.0f, 0.0f, _rotZAxis);
	childTransform_->AddLocation(0.0f, 0.0f);
}
