#include "PreCompile.h"
#include "Boomerang.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include <GameEngineBase\GameEngineSoundPlayer.h>

Boomerang::Boomerang()
	: renderer_(nullptr)
	, collision_(nullptr)
	, sound_(nullptr)
{

}

Boomerang::~Boomerang()
{

}

void Boomerang::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Boomerang", 0.0416f);
	renderer_->SetPivot(eImagePivot::CENTER);
	renderer_->ChangeAnimation("Boomerang");

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(120.f, 100.f);

	state_.CreateState(MakeState(Boomerang, Spawn));
	state_.CreateState(MakeState(Boomerang, FlyLeft));
	state_.CreateState(MakeState(Boomerang, FlyRight));
	state_.CreateState(MakeState(Boomerang, Death));
	state_.ChangeState("Spawn");

	sound_ = std::make_unique<GameEngineSoundPlayer>("sfx_flower_boomerang_projectile.wav");
}

void Boomerang::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (!sound_->IsPlaying())
	{
		sound_->Play();
	}
}

void Boomerang::startSpawn(float _deltaTime)
{
	renderer_->ChangeAnimation("Boomerang");
	transform_->SetLocation(SPAWN_LOCATION);
}

void Boomerang::updateSpawn(float _deltaTime)
{
	if (state_.GetTime() > 0.3f)
	{
		state_ << "FlyLeft";
	}
}

void Boomerang::startFlyLeft(float _deltaTime)
{
}

void Boomerang::updateFlyLeft(float _deltaTime)
{
	transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f, 0.0f);

	if (transform_->GetWorldLocation().x < -500.f)
	{
		transform_->AddLocation(0.0f, -200.f, -0.2f);
		state_ << "FlyRight";
	}
}

void Boomerang::startFlyRight(float _deltaTime)
{
}

void Boomerang::updateFlyRight(float _deltaTime)
{
	transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f, 0.0f);

	if (transform_->GetWorldLocation().x > 1800.f)
	{
		state_ << "Death";
	}
}

void Boomerang::startDeath(float _deltaTime)
{
}

void Boomerang::updateDeath(float _deltaTime)
{
	Release();
}
