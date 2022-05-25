#include "PreCompile.h"
#include "GatlingSeedBlue.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "Map.h"

GatlingSeedBlue::GatlingSeedBlue()
	: renderer_(nullptr)
	, seedRenderer_(nullptr)
	, vineRenderer_(nullptr)
	, collision_(nullptr)
{

}

GatlingSeedBlue::~GatlingSeedBlue()
{

}

void GatlingSeedBlue::Start()
{
	SetHP(HP);
	seedRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	seedRenderer_->CreateAnimationFolder("Seed_Blue", 0.0416f);
	seedRenderer_->CreateAnimationFolder("SeedGrow_Blue", 0.0416f, false);
	seedRenderer_->CreateAnimationFolder("VineGrowBurst", 0.0416f, false);
	seedRenderer_->ChangeAnimation("Seed_Blue");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Chomper", 0.0416f);
	renderer_->CreateAnimationFolder("ChomperDeath", 0.0416f, false);
	renderer_->ChangeAnimation("Chomper");
	renderer_->Off();
	pushHitEffectRenderer(renderer_);

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(40.f);
	collision_->SetLocationY(50.f);

	state_.CreateState(MakeState(GatlingSeedBlue, Fall));
	state_.CreateState(MakeState(GatlingSeedBlue, Landing));
	state_.CreateState(MakeState(GatlingSeedBlue, GrowUp));
	state_.CreateState(MakeState(GatlingSeedBlue, Idle));
	state_.CreateState(MakeState(GatlingSeedBlue, Death));
	state_ << "Fall";
}

void GatlingSeedBlue::Update(float _deltaTime)
{
	MonsterBase::Update(_deltaTime);

	state_.Update(_deltaTime);
}

void GatlingSeedBlue::OnHit()
{
	MonsterBase::OnHit();

	if (hp_ < 0)
	{
		collision_->Off();
		state_ << "Death";
	}
}

void GatlingSeedBlue::startFall(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("Seed_Blue");
}

void GatlingSeedBlue::updateFall(float _deltaTime)
{
	transform_->AddLocation(0.0f, FALL_SPEED * _deltaTime);

	if (float4::BLACK == Map::GetColor(transform_))
	{
		state_ << "Landing";
	}
}

void GatlingSeedBlue::startLanding(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("SeedGrow_Blue");
}

void GatlingSeedBlue::updateLanding(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "GrowUp";
	}
}

void GatlingSeedBlue::startGrowUp(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("VineGrowBurst");

}

void GatlingSeedBlue::updateGrowUp(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->CurFrame_ == 3)
	{
		renderer_->On();
		collision_->SetCollisionGroup(eCollisionGroup::Monster);
		collision_->On();
	}

	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}
}

void GatlingSeedBlue::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Chomper");
}

void GatlingSeedBlue::updateIdle(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}
}

void GatlingSeedBlue::startDeath(float _deltaTime)
{
	renderer_->ChangeAnimation("ChomperDeath");
	renderer_->SetPivot(eImagePivot::CENTER);
}

void GatlingSeedBlue::updateDeath(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
