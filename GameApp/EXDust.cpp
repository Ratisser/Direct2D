#include "PreCompile.h"
#include "EXDust.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>
#include "EXDust.h"

EXDust::EXDust()
	: renderer_(nullptr)
	, childTransform_(nullptr)
{

}

EXDust::~EXDust()
{

}

void EXDust::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationY(20.f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("SSDustA", 0.0416f, false);
	renderer_->SetLocationZ(0.01f);
	renderer_->SetPivot(eImagePivot::CENTER);

	renderer_->ChangeAnimation("SSDustA");
}

void EXDust::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}

void EXDust::Initialize(bool bLeft_, float _rotZAxis)
{
}
