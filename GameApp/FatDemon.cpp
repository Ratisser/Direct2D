#include "PreCompile.h"
#include "FatDemon.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>

#include <GameApp\FatDemonSkull.h>

bool FatDemon::LeftExist = false;
bool FatDemon::RightExist = false;

FatDemon::FatDemon()
	: childTransform_(nullptr)
	, handTransform_(nullptr)
	, handRenderer_(nullptr)
	, renderer_(nullptr)
	, collision_(nullptr)
	, bLeft_(false)
	, bShoot_(false)
	, attackDelay_(0.0f)
	, moveTime_(0.0f)
{

}

FatDemon::~FatDemon()
{

}

void FatDemon::Start()
{
	SetHP(MAX_HP);

	transform_->SetLocationZ(0.1f);
	//transform_->SetScale(0.7f);

	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetScale(0.8f);
	handTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	handTransform_->SetLocationY(500.f);

	handRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(handTransform_);
	handRenderer_->CreateAnimationFolder("FatDemonRelease", 0.0416f, false);
	handRenderer_->ChangeAnimation("FatDemonRelease");
	handRenderer_->Off();


	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("FatDemonIntro");
	renderer_->CreateAnimationFolder("FatDemonTransition", 0.034f, false);
	renderer_->CreateAnimationFolder("FatDemonIdle");
	renderer_->CreateAnimationFolder("FatDemonAttack", 0.034f, false);
	renderer_->CreateAnimationFolder("FatDemonDeath", 0.034f, false);
	renderer_->CreateAnimationFolder("FatDemonDeathIdle");
	pushHitEffectRenderer(renderer_);

	renderer_->ChangeAnimation("FatDemonIntro");

	collision_ = CreateTransformComponent<GameEngineCollision>(childTransform_);
	collision_->SetCollisionGroup(eCollisionGroup::Monster);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(300.f);
	collision_->SetLocationY(250.f);

	state_.CreateState(MakeState(FatDemon, Intro));
	state_.CreateState(MakeState(FatDemon, Transition));
	state_.CreateState(MakeState(FatDemon, Idle));
	state_.CreateState(MakeState(FatDemon, Attack));
	state_.CreateState(MakeState(FatDemon, Death));
	state_.CreateState(MakeState(FatDemon, Release));
	state_.ChangeState("Intro");

}

void FatDemon::Update(float _deltaTime)
{
	MonsterBase::Update(_deltaTime);

	if (bLeft_)
	{
		handRenderer_->SetFlip(false, false);
		renderer_->SetFlip(false, false);
	}
	else
	{
		handRenderer_->SetFlip(true, false);
		renderer_->SetFlip(true, false);
	}

	state_.Update(_deltaTime);
}

void FatDemon::OnHit()
{
	MonsterBase::OnHit();

	if (hp_ <= 0)
	{
		state_ << "Death";
		collision_->Off();
	}
}

void FatDemon::startIntro(float _deltaTime)
{
	if (!LeftExist)
	{
		bLeft_ = true;
		LeftExist = true;

	}
	else if (!RightExist)
	{
		bLeft_ = false;
		RightExist = true;
	}
	else
	{
		state_ << "Release";
		return;
	}

	if (bLeft_)
	{
		prevLocation_ = LEFT_SPAWN_LOCATION;
		nextLocation_ = LEFT_START_LOCATION;
	}
	else
	{
		prevLocation_ = RIGHT_SPAWN_LOCATION;
		nextLocation_ = RIGHT_START_LOCATION;

	}

	renderer_->ChangeAnimation("FatDemonIntro");

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 4);
	std::string soundName = "fat_bat_spawn_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void FatDemon::updateIntro(float _deltaTime)
{
	float time = state_.GetTime();
	transform_->SetWorldLocationXY(GameEngineMath::Lerp(prevLocation_, nextLocation_, time, 1.0f));

	if (time > 1.0f)
	{
		state_ << "Transition";
		return;
	}
}

void FatDemon::startTransition(float _deltaTime)
{
	renderer_->ChangeAnimation("FatDemonTransition");
	handRenderer_->On();
	handRenderer_->ChangeAnimation("FatDemonRelease");
}

void FatDemon::updateTransition(float _deltaTime)
{
	if (handRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		handTransform_->AddLocation(0.0f, 200.f * _deltaTime);
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FatDemonIdle");
	}

	if (state_.GetTime() > 1.0f)
	{
		state_ << "Idle";
	}
}

void FatDemon::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FatDemonIdle");
	handRenderer_->Off();

	GameEngineRandom random;

	attackDelay_ = random.RandomFloat(3.0f, 5.0f);
}

void FatDemon::updateIdle(float _deltaTime)
{
	float time = state_.GetTime();
	moveTime_ += _deltaTime;
	childTransform_->SetLocationY(sinf(moveTime_) * 200.f);

	if (time > attackDelay_)
	{
		state_ << "Attack";
	}
}

void FatDemon::startAttack(float _detlaTime)
{
	renderer_->ChangeAnimation("FatDemonAttack");

	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_frogs_tall_spit_shoot_03.wav");

	bShoot_ = false;
}

void FatDemon::updateAttack(float _detlaTime)
{
	if (renderer_->GetCurrentAnimation()->CurFrame_ == 3)
	{
		if (!bShoot_)
		{
			bShoot_ = true;

			FatDemonSkull* skull = level_->CreateActor<FatDemonSkull>();
			if (bLeft_)
			{
				skull->GetTransform()->SetWorldLocation(childTransform_->GetWorldLocation() + float4(300.f, 320.f, 0.01f));
			}
			else
			{
				skull->GetTransform()->SetWorldLocation(childTransform_->GetWorldLocation() + float4(-300.f, 320.f, 0.01f));
			}
			skull->Init(bLeft_);
		}
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		return;
	}
}

void FatDemon::startDeath(float _deltaTime)
{
	renderer_->ChangeAnimation("FatDemonDeath");
	handRenderer_->Off();

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 3);
	std::string soundName = "fat_bat_die_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void FatDemon::updateDeath(float _deltaTime)
{
	transform_->AddLocation(0.0f, -1000.f * _deltaTime);

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FatDemonDeathIdle");
	}

	if (state_.GetTime() > 2.0f)
	{
		state_ << "Release";
	}
}

void FatDemon::startRelease(float _deltaTime)
{
	if (bLeft_)
	{
		LeftExist = false;
	}
	else
	{
		RightExist = false;
	}
}

void FatDemon::updateRelease(float _deltaTime)
{
	Release();
}
