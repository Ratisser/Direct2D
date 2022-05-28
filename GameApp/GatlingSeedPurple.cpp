#include "PreCompile.h"
#include "GatlingSeedPurple.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "Map.h"
#include "FlowerLevel.h"
#include "Player.h"


GatlingSeedPurple::GatlingSeedPurple()
	: renderer_(nullptr)
	, seedRenderer_(nullptr)
	, vineRenderer_(nullptr)
	, collision_(nullptr)
	, vineTransform_(nullptr)
	, direction_(float4::LEFT)
	, timeCounter_(0.0f)
{

}

GatlingSeedPurple::~GatlingSeedPurple()
{

}

void GatlingSeedPurple::Start()
{
	SetHP(HP);
	seedRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	seedRenderer_->CreateAnimationFolder("Seed_Purple", 0.0416f);
	seedRenderer_->CreateAnimationFolder("SeedGrow_Purple", 0.0416f, false);
	seedRenderer_->CreateAnimationFolder("VineGrowBurst", 0.0416f, false);
	seedRenderer_->ChangeAnimation("Seed_Purple");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("MiniFlowerSpawn", 0.0416f, false);
	renderer_->CreateAnimationFolder("MiniFlowerFly", 0.067f);
	renderer_->CreateAnimationFolder("MiniFlowerDeath", 0.0416f, false);
	renderer_->CreateAnimationFolder("MiniFlowerSpit", 0.0416f, false);
	renderer_->CreateAnimationFolderReverse("MiniFlowerSpitReverse", "MiniFlowerSpit", 0.0416f, false);
	renderer_->ChangeAnimation("MiniFlowerSpawn");
	renderer_->SetPivot(eImagePivot::CENTER);
	renderer_->Off();
	pushHitEffectRenderer(renderer_);

	vineTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	vineTransform_->SetLocationZ(0.001f);

	vineRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(vineTransform_);
	vineRenderer_->CreateAnimationFolder("Vine", 0.0416f, false);
	vineRenderer_->ChangeAnimation("Vine");
	vineRenderer_->Off();

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(40.f);
	collision_->SetLocationY(30.f);

	state_.CreateState(MakeState(GatlingSeedPurple, Fall));
	state_.CreateState(MakeState(GatlingSeedPurple, Landing));
	state_.CreateState(MakeState(GatlingSeedPurple, GrowUp));
	state_.CreateState(MakeState(GatlingSeedPurple, FlyUp));
	state_.CreateState(MakeState(GatlingSeedPurple, Spawn));
	state_.CreateState(MakeState(GatlingSeedPurple, Idle));
	state_.CreateState(MakeState(GatlingSeedPurple, Fire));
	state_.CreateState(MakeState(GatlingSeedPurple, FireEnd));
	state_.CreateState(MakeState(GatlingSeedPurple, Death));
	state_ << "Fall";
}

void GatlingSeedPurple::Update(float _deltaTime)
{
	MonsterBase::Update(_deltaTime);

	state_.Update(_deltaTime);
}

void GatlingSeedPurple::OnHit()
{
	MonsterBase::OnHit();

	if (hp_ <= 0)
	{
		collision_->Off();
		state_ << "Death";
	}
}

void GatlingSeedPurple::startFall(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("Seed_Purple");
}

void GatlingSeedPurple::updateFall(float _deltaTime)
{
	transform_->AddLocation(0.0f, FALL_SPEED * _deltaTime);

	if (float4::BLACK == Map::GetColor(transform_))
	{
		state_ << "Landing";
		collision_->Off();
		collision_->SetCollisionGroup(eCollisionGroup::Monster);
	}
}

void GatlingSeedPurple::startLanding(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("SeedGrow_Purple");
}

void GatlingSeedPurple::updateLanding(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "GrowUp";
	}
}

void GatlingSeedPurple::startGrowUp(float _deltaTime)
{
	seedRenderer_->ChangeAnimation("VineGrowBurst");
	vineTransform_->SetWorldLocation(transform_->GetWorldLocation());
	vineTransform_->AddLocation(0.0f, 0.0f, 0.001f);
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_venus_vine_grow_medium.wav");
}

void GatlingSeedPurple::updateGrowUp(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->CurFrame_ == 3)
	{
		vineRenderer_->On();
	}

	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}

	if (vineRenderer_->GetCurrentAnimation()->CurFrame_ == 12)
	{
		state_ << "Spawn";
	}
}

void GatlingSeedPurple::startSpawn(float _deltaTime)
{
	transform_->AddLocation(0.0f, 234.f);
	renderer_->On();
}

void GatlingSeedPurple::updateSpawn(float _deltaTime)
{
	if (vineRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineRenderer_->Off();
	}

	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "FlyUp";
	}
}

void GatlingSeedPurple::startFlyUp(float _deltaTime)
{
	renderer_->ChangeAnimation("MiniFlowerFly");
	collision_->On();
	collision_->SetLocation(0.0f, 0.0f);
	collision_->SetScale(60.f);

	prevLocation_ = transform_->GetWorldLocation();
	nextLocation_ = prevLocation_;
	nextLocation_.y = -100;
	timeCounter_ = 0.0f;
}

void GatlingSeedPurple::updateFlyUp(float _deltaTime)
{
	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}

	if (vineRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineRenderer_->Off();
	}

	if (state_.GetTime() < 1.0f)
	{
		transform_->SetWorldLocation(GameEngineMath::Lerp(prevLocation_, nextLocation_, state_.GetTime(), 1.0f));
	}

	if (state_.GetTime() >= 1.0f)
	{
		transform_->SetWorldLocation(GameEngineMath::Lerp(nextLocation_, float4(500.f, -100, nextLocation_.z), state_.GetTime() - 1.0f, 1.0f));
	}

	if (state_.GetTime() > 2.0f)
	{
		state_ << "Idle";
	}
}

void GatlingSeedPurple::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("MiniFlowerFly");
}

void GatlingSeedPurple::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}

	if (vineRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineRenderer_->Off();
	}

	transform_->SetLocationX(sinf(timeCounter_) * 300.f + 500.f);

	if (state_.GetTime() > 4.0f)
	{
		state_ << "Fire";
	}
}

void GatlingSeedPurple::startFire(float _deltaTime)
{
	renderer_->ChangeAnimation("MiniFlowerSpit");
}

void GatlingSeedPurple::updateFire(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "FireEnd";
	}
}

void GatlingSeedPurple::startFireEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("MiniFlowerSpitReverse");
}

void GatlingSeedPurple::updateFireEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
	}
}

void GatlingSeedPurple::startDeath(float _deltaTime)
{
	renderer_->ChangeAnimation("MiniFlowerDeath");
	renderer_->SetPivot(eImagePivot::CENTER);
}

void GatlingSeedPurple::updateDeath(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
