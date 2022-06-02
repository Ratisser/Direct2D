#include "PreCompile.h"
#include "LandDust.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

LandDust::LandDust()
	: renderer_(nullptr)
	, childTransform_(nullptr)
{

}

LandDust::~LandDust()
{

}

void LandDust::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationY(20.f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("LandDust", 0.034f, false);
	renderer_->SetLocationZ(0.01f);
	renderer_->SetPivot(eImagePivot::CENTER);

	renderer_->ChangeAnimation("LandDust");
}

void LandDust::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
