#include "PreCompile.h"
#include "GatlingSeedPink.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "Map.h"

GatlingSeedPink::GatlingSeedPink()
	: renderer_(nullptr)
	, seedRenderer_(nullptr)
	, vineRenderer_(nullptr)
	, collision_(nullptr)
{

}

GatlingSeedPink::~GatlingSeedPink()
{

}

void GatlingSeedPink::Start()
{
	SetHP(HP);
	seedRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	seedRenderer_->CreateAnimationFolder("Seed_Pink", 0.0416f);
	seedRenderer_->CreateAnimationFolder("SeedGrow_Pink", 0.0416f, false);
	seedRenderer_->CreateAnimationFolder("VineGrowBurst", 0.0416f, false);
	seedRenderer_->ChangeAnimation("Seed_Pink");

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

	state_.CreateState(MakeState(GatlingSeedPink, Fall));
	state_.CreateState(MakeState(GatlingSeedPink, Landing));
	state_.CreateState(MakeState(GatlingSeedPink, GrowUp));
	state_.CreateState(MakeState(GatlingSeedPink, Idle));
	state_.CreateState(MakeState(GatlingSeedPink, Death));
	state_ << "Fall";
}

void GatlingSeedPink::Update(float _deltaTime)
{
	MonsterBase::Update(_deltaTime);

	state_.Update(_deltaTime);
}

void GatlingSeedPink::OnHit()
{
	MonsterBase::OnHit();

	if (hp_ < 0)
	{
		collision_->Off();
		state_ << "Death";
	}
}

void GatlingSeedPink::startFall(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("Seed_Pink");
}

void GatlingSeedPink::updateFall(float _deltaTime)
{
	transform_->AddLocation(0.0f, FALL_SPEED * _deltaTime);

	if (float4::BLACK == Map::GetColor(transform_))
	{
		state_ << "Landing";
	}
}

void GatlingSeedPink::startLanding(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("SeedGrow_Pink");
}

void GatlingSeedPink::updateLanding(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "GrowUp";
	}
}

void GatlingSeedPink::startGrowUp(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("VineGrowBurst");

}

void GatlingSeedPink::updateGrowUp(float _deltaTime)
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

void GatlingSeedPink::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Chomper");
}

void GatlingSeedPink::updateIdle(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}
}

void GatlingSeedPink::startDeath(float _deltaTime)
{
	renderer_->ChangeAnimation("ChomperDeath");
	renderer_->SetPivot(eImagePivot::CENTER);
}

void GatlingSeedPink::updateDeath(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
