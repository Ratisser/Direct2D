#include "PreCompile.h"
#include "BombBat.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineImageRenderer.h>

BombBat::BombBat()
	: renderer_(nullptr)
	, bLeft_(false)
	, timeCounter_(0.0f)
	, flyTime_(0.0f)
	, horizontalDirectionChangeDelay_(0.0f)
{

}

BombBat::~BombBat()
{

}

void BombBat::Start()
{
	transform_->SetScale(0.8f);

	ParryObjectBase::Start();
	collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	collision_->SetScale(80.f);
	collision_->SetLocation(20.f, -20.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("BombBat");
	renderer_->CreateAnimationFolder("BombExplosion", 0.034f, false);
	renderer_->ChangeAnimation("BombBat");
	renderer_->SetPivot(eImagePivot::CENTER);

	state_.CreateState("Summon", std::bind(&BombBat::startSummon, this, std::placeholders::_1), std::bind(&BombBat::updateSummon, this, std::placeholders::_1));
	state_.CreateState("Fly", std::bind(&BombBat::startFly, this, std::placeholders::_1), std::bind(&BombBat::updateFly, this, std::placeholders::_1));
	state_.CreateState("Explosion", std::bind(&BombBat::startExplosion, this, std::placeholders::_1), std::bind(&BombBat::updateExplosion, this, std::placeholders::_1));

	state_ << "Summon";

	GameEngineRandom random;
}

void BombBat::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void BombBat::Initialize(bool _bLeft)
{
	bLeft_ = _bLeft;
	if (_bLeft)
	{
		renderer_->SetRotation(0.0f, -20.0f * GameEngineMath::DegreeToRadian, 0.0f);
	}
	else
	{
		renderer_->SetRotation(0.0f, 20.0f * GameEngineMath::DegreeToRadian, 0.0f);
	}
}

void BombBat::onParry()
{
	Release();
}

void BombBat::startSummon(float _deltaTime)
{
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_bomb_appear.wav");
	prevRotation_ = renderer_->GetRotation();
	timeCounter_ = 0.0f;
}

void BombBat::updateSummon(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (bLeft_)
	{
		transform_->AddLocation(-300.f * _deltaTime, 0.0f);
	}
	else
	{
		transform_->AddLocation(300.f * _deltaTime, 0.0f);
	}

	renderer_->SetRotation(GameEngineMath::Lerp(prevRotation_, float4::ZERO, timeCounter_, 0.1f));

	if (1.0f < timeCounter_)
	{
		state_ << "Fly";
		return;
	}
}

void BombBat::startFly(float _deltaTime)
{
	timeCounter_ = 0.0f;
	flyTime_ = 0.0f;
	horizontalDirectionChangeDelay_ = 0.0f;

	moveSpeed_.y = 500.f;
}

void BombBat::updateFly(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	flyTime_ += _deltaTime;
	horizontalDirectionChangeDelay_ += _deltaTime;

	if (flyTime_ > 0.5f)
	{

		moveSpeed_.y = 500.f;
		flyTime_ = 0.0f;
	}

	if (horizontalDirectionChangeDelay_ > 0.2f)
	{
		GameEngineRandom random;
		moveSpeed_.x = random.RandomFloat(0.f, 600.f) - 300.f;
		horizontalDirectionChangeDelay_ = 0.0f;
	}

	moveSpeed_.y -= 2000.f * _deltaTime;

	transform_->AddLocation(moveSpeed_ * _deltaTime);

	if (timeCounter_ > 3.0f)
	{
		state_ << "Explosion";
		return;
	}
}

void BombBat::startExplosion(float _deltaTime)
{
	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 2);
	std::string soundName = "sfx_player_plane_shmup_bomb_explode_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

	renderer_->ChangeAnimation("BombExplosion");
	SetParryable(false);
	collision_->SetParent(renderer_);
	collision_->SetScale(0.7f);
}

void BombBat::updateExplosion(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
