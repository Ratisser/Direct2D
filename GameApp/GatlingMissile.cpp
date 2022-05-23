#include "PreCompile.h"
#include "GatlingMissile.h"

#include <GameEngineBase\GameEngineRandom.h>

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

#include <GameApp\GatlingSmokeEffect.h>

GatlingMissile::GatlingMissile()
	: renderer_(nullptr)
	, animIndex_(0)
	, time_(0.0f)
{

}

GatlingMissile::~GatlingMissile()
{

}

void GatlingMissile::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("GatlingMissileBlue", 3.0f, false);
	renderer_->CreateAnimationFolder("GatlingMissilePurple", 3.0f, false);


	GameEngineRandom random;
	bool bBlue = random.RandomBool();

	if (bBlue)
	{
		renderer_->ChangeAnimation("GatlingMissileBlue");
	}
	else
	{
		renderer_->ChangeAnimation("GatlingMissilePurple");
	}

	animIndex_ = random.RandomInt(0, 5);
	renderer_->GetCurrentAnimation()->CurFrame_ = animIndex_;

	if (animIndex_ < 4)
	{
		GameEngineActor* smokeEffect = level_->CreateActor<GatlingSmokeEffect>();
		smokeEffect->GetTransform()->SetLocation(1120.f + animIndex_ * 5.f, -140.f, 0.1f);
	}

}

void GatlingMissile::Update(float _deltaTime)
{
	time_ += _deltaTime;
	if (time_ > 0.067f)
	{
		Release();
	}
}
