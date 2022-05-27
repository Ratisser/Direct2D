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
	seedRenderer_->CreateAnimationFolder("Seed_Blue", 0.0416f);
	seedRenderer_->CreateAnimationFolder("SeedGrow_Blue", 0.0416f, false);
	seedRenderer_->CreateAnimationFolder("VineGrowBurst", 0.0416f, false);
	seedRenderer_->ChangeAnimation("Seed_Blue");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("VenusSpawn", 0.0416f, false);
	renderer_->CreateAnimationFolder("VenusLoop", 0.067f);
	renderer_->CreateAnimationFolder("VenusDeath", 0.0416f, false);
	renderer_->ChangeAnimation("VenusSpawn");
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
	state_.CreateState(MakeState(GatlingSeedPurple, Spawn));
	state_.CreateState(MakeState(GatlingSeedPurple, Idle));
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
	seedRenderer_->ChangeAnimation("Seed_Blue");
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
	seedRenderer_->ChangeAnimation("SeedGrow_Blue");
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
		state_ << "Idle";
	}
}

void GatlingSeedPurple::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("VenusLoop");
	collision_->On();
	collision_->SetLocation(0.0f, 0.0f);
	collision_->SetScale(60.f);
}

void GatlingSeedPurple::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (renderer_->GetCurrentAnimation()->CurFrame_ == 3 && timeCounter_ > 0.07f)
	{
		GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_venus_a_chomp.wav");
		timeCounter_ = 0.0f;
	}

	if (seedRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		seedRenderer_->Off();
	}

	if (vineRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineRenderer_->Off();
	}

	transform_->AddLocation(direction_ * MOVE_SPEED * _deltaTime);

	if (state_.GetTime() < 1.0f)
	{
		GameEngineActor* player = level_->GetLevel<FlowerLevel>()->GetPlayer();
		float4 playerLocation = player->GetTransform()->GetWorldLocation();
		playerLocation.y += 50.f;
		float4 venusLocation = transform_->GetWorldLocation();

		if (playerLocation.x < venusLocation.x)
		{
			float4 destDirection = playerLocation - venusLocation;
			destDirection.Normalize3D();

			if (abs(destDirection.y - direction_.y) > 0.001f)
			{
				renderer_->AddRotation(0.0f, 0.0f, -destDirection.y * 3.0f * _deltaTime);
				direction_ = float4::RotateZRadian(direction_, -destDirection.y * 3.0f * _deltaTime);
			}
		}
	}

	if (state_.GetTime() > 5.0f)
	{
		Release();
	}
}

void GatlingSeedPurple::startDeath(float _deltaTime)
{
	renderer_->ChangeAnimation("VenusDeath");
	renderer_->SetPivot(eImagePivot::CENTER);
}

void GatlingSeedPurple::updateDeath(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
