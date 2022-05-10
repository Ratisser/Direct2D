#include "PreCompile.h"
#include "Axe.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "DevilLevel.h"
#include "Player.h"

Axe::Axe()
	: renderer_(nullptr)
	, collision_(nullptr)
	, childTransform_(nullptr)
	, timeCounter_(0.0f)
{

}

Axe::~Axe()
{

}

void Axe::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationZ(-0.02f);
	
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->SetPivot(eImagePivot::CENTER);

	renderer_->CreateAnimationFolder("Spiral_Spawn", 0.034f, false);
	renderer_->CreateAnimationFolder("Spiral_Spin");
	renderer_->ChangeAnimation("Spiral_Spawn");

	collision_ = CreateTransformComponent<GameEngineCollision>(childTransform_);
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(50.f);
	collision_->Off();

	state_.CreateState("Summon", std::bind(&Axe::startSummon, this, std::placeholders::_1), std::bind(&Axe::updateSummon, this, std::placeholders::_1));
	state_.CreateState("SeekPlayer", std::bind(&Axe::startSeekPlayer, this, std::placeholders::_1), std::bind(&Axe::updateSeekPlayer, this, std::placeholders::_1));
	state_.CreateState("Attack", std::bind(&Axe::startAttack, this, std::placeholders::_1), std::bind(&Axe::updateAttack, this, std::placeholders::_1));
	state_.CreateState("End", std::bind(&Axe::startEnd, this, std::placeholders::_1), std::bind(&Axe::updateEnd, this, std::placeholders::_1));
	state_ << "Summon";
}

void Axe::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void Axe::startSummon(float _deltaTime)
{
	renderer_->ChangeAnimation("Spiral_Spawn", true);
	timeCounter_ = 0.0f;
}

void Axe::updateSummon(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	
	if (timeCounter_ < 0.5f)
	{
		return;
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "SeekPlayer";
		return;
	}
}

void Axe::startSeekPlayer(float _deltaTime)
{
	DevilLevel* devilLevel = dynamic_cast<DevilLevel*>(level_);
	if (nullptr != devilLevel)
	{
		GameEngineActor* player = devilLevel->GetPlayer();
		seekDirection_ = player->GetTransform()->GetWorldLocation();
		seekDirection_ -= transform_->GetWorldLocation();

		seekDirection_.Normalize3D();
	}

	collision_->On();
	renderer_->ChangeAnimation("Spiral_Spin");

	timeCounter_ = 0.0f;
}

void Axe::updateSeekPlayer(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	transform_->AddLocation(seekDirection_ * SPIN_SPEED * 2.f * _deltaTime);

	if (timeCounter_ > 0.7f)
	{
		state_ << "Attack";
		return;
	}
}

void Axe::startAttack(float _deltaTime)
{
	timeCounter_ = 0.0f;
}

void Axe::updateAttack(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	transform_->AddRotation(0.0f, 0.0f, SPIN_SPEED * GameEngineMath::DegreeToRadian * _deltaTime);
	childTransform_->AddRotation(0.0f, 0.f, -SPIN_SPEED * GameEngineMath::DegreeToRadian * _deltaTime);

	childTransform_->AddLocation(RADIUS_SPEED * _deltaTime, 0.f);

	if (timeCounter_ > 5.0f)
	{
		state_ << "End";
	}
}

void Axe::startEnd(float _deltaTime)
{
}

void Axe::updateEnd(float _deltaTime)
{
	Release();
}

