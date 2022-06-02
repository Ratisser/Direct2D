#include "PreCompile.h"
#include "HitFX.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

HitFX::HitFX()
	: renderer_(nullptr)
	, childTransform_(nullptr)
{

}

HitFX::~HitFX()
{

}

void HitFX::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationY(-200.f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("HitFX01", 0.0416f, false);
	renderer_->CreateAnimationFolder("HitFX02", 0.0416f, false);
	renderer_->CreateAnimationFolder("HitFX03", 0.0416f, false);
	renderer_->SetLocationZ(0.01f);

	GameEngineRandom random;
	int num = random.RandomInt(1, 3);

	renderer_->ChangeAnimation("HitFX0" + std::to_string(num));
}

void HitFX::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
