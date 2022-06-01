#include "PreCompile.h"
#include "Acorn.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "Map.h"
#include "FlowerLevel.h"
#include "Player.h"

int Acorn::RefCount = 0;

Acorn::Acorn()
	: renderer_(nullptr)
	, effect_(nullptr)
	, effectTransform_(nullptr)
	, collision_(nullptr)
	, velocity_(0.0f)
{
	RefCount++;
	delay_ = RefCount * 0.5f + 1.0f;
	height_ = -160.f - RefCount * 100.f;
}

Acorn::~Acorn()
{
	RefCount--;
}

void Acorn::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->SetPivot(eImagePivot::CENTER);

	renderer_->CreateAnimationFolder("AcornSpawn");
	renderer_->CreateAnimationFolder("AcornFly");
	renderer_->CreateAnimationFolder("AcornDeath", 0.0416f, false);
	renderer_->ChangeAnimation("AcornSpawn");
	
	effectTransform_ = CreateTransformComponent<GameEngineTransformComponent>();

	effect_ = CreateTransformComponent<GameEngineImageRenderer>(effectTransform_);
	effect_->CreateAnimationFolder("AcornEffect", 0.0416f, false);
	effect_->SetPivot(eImagePivot::RIGHT);
	effect_->ChangeAnimation("AcornEffect");
	effect_->Off();

	collision_ = CreateTransformComponent<GameEngineCollision>(renderer_);
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(0.9f);

	state_.CreateState(MakeState(Acorn, Spawn));
	state_.CreateState(MakeState(Acorn, Fly));
	state_.CreateState(MakeState(Acorn, Death));
	state_ << "Spawn";

}

void Acorn::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void Acorn::startSpawn(float _deltaTime)
{
	renderer_->ChangeAnimation("AcornSpawn");
	transform_->SetWorldLocation(1000.f, height_, 1.1f);
}

void Acorn::updateSpawn(float _deltaTime)
{
	if (state_.GetTime() > delay_)
	{
		state_ << "Fly";
	}
}

void Acorn::startFly(float _deltaTime)
{
	float4 playerLocation = level_->GetLevel<FlowerLevel>()->GetPlayer()->GetTransform()->GetWorldLocation();
	float4 acornLocation = transform_->GetWorldLocation();
	playerLocation.z = 0.0f;
	acornLocation.z = 0.0f;

	direction_ = playerLocation - acornLocation;
	direction_.Normalize3D();

	if (acornLocation.y - playerLocation.y < 0)
	{
		effect_->AddRotation(0.0f, 0.0f, -acos(direction_.x * -1.f));
		renderer_->AddRotation(0.0f, 0.0f, -acos(direction_.x * -1.f));
	}
	else
	{
		effect_->AddRotation(0.0f, 0.0f, acos(direction_.x * -1.f));
		renderer_->AddRotation(0.0f, 0.0f, acos(direction_.x * -1.f));
	}

	renderer_->ChangeAnimation("AcornFly");

	effectTransform_->AddLocation(direction_ * -100.f);
	effect_->On();
}

void Acorn::updateFly(float _deltaTime)
{
	if (effect_->GetCurrentAnimation()->IsEnd_)
	{
		effect_->Off();
	}

	const float acceleration = 500.f;
	velocity_ += acceleration * _deltaTime;
	transform_->AddLocation(direction_ * velocity_ * _deltaTime);

	if (float4::BLACK == Map::GetColor(transform_))
	{
		state_ << "Death";
	}
}

void Acorn::startDeath(float _deltaTime)
{
	renderer_->ChangeAnimation("AcornDeath");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_bullet_seed_poof.wav");
	collision_->Off();
}

void Acorn::updateDeath(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
