#include "PreCompile.h"
#include "DevilTear.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

DevilTear::DevilTear()
	: renderer_(nullptr)
	, timeCounter_(0.0f)
{

}

DevilTear::~DevilTear()
{

}

void DevilTear::Start()
{
	transform_->SetLocationZ(0.045f);

	ParryObjectBase::Start();
	collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	collision_->SetScale(45.f, 85.f);
	collision_->SetLocationY(42.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("DevilTear1", 0.0416f);
	renderer_->CreateAnimationFolder("DevilTear2", 0.0416f);

	GameEngineRandom random;

	if (random.RandomInt(0, 1))
	{
		renderer_->ChangeAnimation("DevilTear1");
	}
	else
	{
		renderer_->ChangeAnimation("DevilTear2");
	}

}

void DevilTear::Update(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	transform_->AddLocation(0.0f, FALL_SPEED * _deltaTime);

	if (timeCounter_ > 5.0f)
	{
		Release();
	}
}

void DevilTear::onParry()
{
	Release();
}
