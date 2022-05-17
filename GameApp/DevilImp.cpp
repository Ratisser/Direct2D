#include "PreCompile.h"
#include "DevilImp.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "DevilLevel.h"
#include "Player.h"

int DevilImp::refCounter_ = 0;

DevilImp::DevilImp()
	: childTransform_(nullptr)
	, spawnRenderer_(nullptr)
	, renderer_(nullptr)
	, collision_(nullptr)
	, nextAttackDelay_(0.0f)
	, accelerationY_(0.0f)
{
	refCounter_++;
}

DevilImp::~DevilImp()
{
	refCounter_--;
}

void DevilImp::Start()
{
	SetHP(HP);

	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();

	spawnRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	spawnRenderer_->CreateAnimationFolder("SpawnImp", 0.034f, false);
	spawnRenderer_->ChangeAnimation("SpawnImp");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("ImpFlyingUp", 0.067f);
	renderer_->CreateAnimationFolder("ImpFlying");
	renderer_->CreateAnimationFolder("ImpAttack");
	renderer_->CreateAnimationFolder("ImpAttackEnd", 0.034f, false);
	renderer_->CreateAnimationFolder("Explosion", 0.034f, false);
	renderer_->ChangeAnimation("ImpFlyingUp");
	renderer_->Off();
	pushHitEffectRenderer(renderer_);

	collision_ = CreateTransformComponent<GameEngineCollision>(childTransform_);
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
	MonsterBase::Update(_deltaTime);

	state_.Update(_deltaTime);
}

void DevilImp::OnHit()
{
	MonsterBase::OnHit();
	if (hp_ < 1)
	{
		state_ << "Death";
	}
}

void DevilImp::startSpawn(float _deltaTime)
{
	renderer_->ChangeAnimation("ImpFlyingUp", true);
	spawnRenderer_->ChangeAnimation("SpawnImp", true);

	GameEngineRandom random;
	float x, y;
	x = random.RandomFloat(SPAWN_MIN_X, SPAWN_MAX_X);
	y = -random.RandomFloat(SPAWN_MIN_Y, SPAWN_MAX_Y);
	childTransform_->SetLocation(x, y);

	int soundNumber = random.RandomInt(1, 3);
	std::string soundName = "sfx_devil_imp_spawn_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
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
	childTransform_->AddLocation(0.0f, 200.f * _deltaTime);
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
	prevLocation_ = childTransform_->GetWorldLocation();
	nextLocation_.x = x;
	nextLocation_.y = FLY_HEIGHT;
	nextAttackDelay_ = random.RandomFloat(2.0f, 5.0f);

	collision_->On();
}

void DevilImp::updateFly(float _deltaTime)
{
	float4 location = GameEngineMath::Lerp(prevLocation_, nextLocation_, state_.GetTime(), 1.0f);

	childTransform_->SetWorldLocationXY(location.x, location.y);

	if (state_.GetTime() > nextAttackDelay_)
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

	speed_ = player->GetTransform()->GetWorldLocation() - childTransform_->GetWorldLocation();
	speed_.z = 0.0f;
	speed_.y *= 1.5f;
	accelerationY_ = -speed_.y;

	GameEngineSoundManager::GetInstance().PlaySoundByName("sxf_level_devil_bat_bomb_spin_01.wav");
}

void DevilImp::updateAttack(float _deltaTime)
{
	childTransform_->AddLocation(speed_ * _deltaTime);

	speed_.y += accelerationY_ * _deltaTime;

	if (state_.GetTime() > 2.5f)
	{
		state_ << "EndAttack";
		return;
	}
}

void DevilImp::startEndAttack(float _deltaTime)
{
	GameEngineRandom random;
	float x = random.RandomFloat(FLY_MIN_X, FLY_MAX_X);
	prevLocation_ = childTransform_->GetWorldLocation();
	nextLocation_.x = x;
	nextLocation_.y = FLY_HEIGHT;
}

void DevilImp::updateEndAttack(float _deltaTime)
{
	float4 location = GameEngineMath::Lerp(prevLocation_, nextLocation_, state_.GetTime(), 1.0f);

	childTransform_->SetWorldLocationXY(location.x, location.y);

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("ImpFlying");
		GameEngineRandom random;
		int soundNumber = random.RandomInt(1, 2);
		std::string soundName = "sxf_level_devil_bat_bomb_spin_stop_0" + std::to_string(soundNumber) + ".wav";
		GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
	}

	if (state_.GetTime() > 1.0f && renderer_->GetCurrentAnimation()->Name_ == "ImpAttack")
	{
		renderer_->ChangeAnimation("ImpAttackEnd");
	}

	if (state_.GetTime() > 3.0f)
	{
		state_ << "Attack";
		return;
	}


}

void DevilImp::startDeath(float _deltaTime)
{
	childTransform_->SetScale(0.5f);
	childTransform_->AddLocation(0.0f, 50.f, 0.0f);
	renderer_->ChangeAnimation("Explosion");
	renderer_->SetPivot(eImagePivot::CENTER);
	collision_->Off();

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 4);
	std::string soundName = "sfx_level_flying_bird_smallbird_death_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void DevilImp::updateDeath(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Release";
		return;
	}
}

void DevilImp::startRelease(float _deltaTime)
{
}

void DevilImp::updateRelease(float _deltaTime)
{
	Release();
}