#include "PreCompile.h"
#include "DevilImp.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "DevilLevel.h"
#include "Player.h"

DevilImp::DevilImp()
	: spawnRenderer_(nullptr)
	, renderer_(nullptr)
	, collision_(nullptr)
	, accelerationY_(0.0f)
{

}

DevilImp::~DevilImp()
{

}

void DevilImp::Start()
{
	spawnRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	spawnRenderer_->CreateAnimationFolder("SpawnImp", 0.034f, false);
	spawnRenderer_->ChangeAnimation("SpawnImp");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("ImpFlyingUp", 0.067f);
	renderer_->CreateAnimationFolder("ImpFlying");
	renderer_->CreateAnimationFolder("ImpAttack");
	renderer_->CreateAnimationFolder("ImpAttackEnd", 0.034f, false);
	renderer_->ChangeAnimation("ImpFlyingUp");
	renderer_->SetLocationZ(0.1f);
	renderer_->Off();

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Monster);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(100.f);
	collision_->SetLocationY(50.f);
	collision_->Off();

	state_.CreateState("Spawn", std::bind(&DevilImp::startSpawn, this, std::placeholders::_1), std::bind(&DevilImp::updateSpawn, this, std::placeholders::_1));
	state_.CreateState("FlyUp", std::bind(&DevilImp::startFlyUp, this, std::placeholders::_1), std::bind(&DevilImp::updateFlyUp, this, std::placeholders::_1));
	state_.CreateState("Fly", std::bind(&DevilImp::startFly, this, std::placeholders::_1), std::bind(&DevilImp::updateFly, this, std::placeholders::_1));
	state_.CreateState("Attack", std::bind(&DevilImp::startAttack, this, std::placeholders::_1), std::bind(&DevilImp::updateAttack, this, std::placeholders::_1));
	state_.CreateState("EndAttack", std::bind(&DevilImp::startEndAttack, this, std::placeholders::_1), std::bind(&DevilImp::updateEndAttack, this, std::placeholders::_1));
	state_.CreateState("Death", std::bind(&DevilImp::startDeath, this, std::placeholders::_1), std::bind(&DevilImp::updateDeath, this, std::placeholders::_1));
	state_.CreateState("Release", std::bind(&DevilImp::startRelease, this, std::placeholders::_1), std::bind(&DevilImp::updateRelease, this, std::placeholders::_1));

	state_ << "Spawn";
}

void DevilImp::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void DevilImp::startSpawn(float _deltaTime)
{
	renderer_->ChangeAnimation("ImpFlyingUp", true);
	spawnRenderer_->ChangeAnimation("SpawnImp", true);

	GameEngineRandom random;
	float x, y;
	x = random.RandomFloat(SPAWN_MIN_X, SPAWN_MAX_X);
	y = -random.RandomFloat(SPAWN_MIN_Y, SPAWN_MAX_Y);
	transform_->SetLocation(x, y);
}

void DevilImp::updateSpawn(float _deltaTime)
{
	if (spawnRenderer_->GetCurrentAnimation()->CurFrame_ == 10)
	{
		renderer_->On();
	}

	if (spawnRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		spawnRenderer_->Off();
		state_ << "FlyUp";
		return;
	}
}

void DevilImp::startFlyUp(float _deltaTime)
{
}

void DevilImp::updateFlyUp(float _deltaTime)
{
	transform_->AddLocation(0.0f, 100.f * _deltaTime);
	if (state_.GetTime() > 2.0f)
	{
		state_ << "Fly";
		return;
	}
}

void DevilImp::startFly(float _deltaTime)
{
	renderer_->ChangeAnimation("ImpFlying");

	GameEngineRandom random;
	float x = random.RandomFloat(FLY_MIN_X, FLY_MAX_X);
	prevLocation_ = transform_->GetWorldLocation();
	nextLocation_.x = x;
	nextLocation_.y = FLY_HEIGHT;
}

void DevilImp::updateFly(float _deltaTime)
{
	transform_->SetLocation(GameEngineMath::Lerp(prevLocation_, nextLocation_, state_.GetTime(), 1.0f));

	if (state_.GetTime() > 3.0f)
	{
		state_ << "Attack";
		return;
	}
}

void DevilImp::startAttack(float _deltaTime)
{
	renderer_->ChangeAnimation("ImpAttack");

	DevilLevel* level = level_->GetLevel<DevilLevel>();
	GameEngineActor* player = level->GetPlayer();

	speed_ = transform_->GetWorldLocation() - player->GetTransform()->GetWorldLocation();
	speed_.z = 0.0f;
	speed_.y *= 2.0f;
	accelerationY_ = -speed_.y;
}

void DevilImp::updateAttack(float _deltaTime)
{
	transform_->AddLocation(speed_);

	speed_.y += accelerationY_ * _deltaTime;
}

void DevilImp::startEndAttack(float _deltaTime)
{
}

void DevilImp::updateEndAttack(float _deltaTime)
{
}

void DevilImp::startDeath(float _deltaTime)
{
}

void DevilImp::updateDeath(float _deltaTime)
{
}

void DevilImp::startRelease(float _deltaTime)
{
}

void DevilImp::updateRelease(float _deltaTime)
{
}

void DevilImp::OnHit()
{
}
