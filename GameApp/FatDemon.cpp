#include "PreCompile.h"
#include "FatDemon.h"

#include <GameEngine\GameEngineImageRenderer.h>

bool FatDemon::Left = false;
bool FatDemon::Right = false;

FatDemon::FatDemon()
	: childTransform_(nullptr)
	, handRenderer_(nullptr)
	, renderer_(nullptr)
	, bLeft_(false)
{

}

FatDemon::~FatDemon()
{

}

void FatDemon::Start()
{
	transform_->SetLocationZ(0.4f);

	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();

	handRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	handRenderer_->CreateAnimationFolder("FatDemonRelease", 0.0416f, false);
	handRenderer_->ChangeAnimation("FatDemonRelease");
	handRenderer_->Off();


	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("FatDemonIntro");
	renderer_->CreateAnimationFolder("FatDemonTransition");
	renderer_->CreateAnimationFolder("FatDemonIdle");
	renderer_->CreateAnimationFolder("FatDemonAttack", 0.034f, false);
	renderer_->CreateAnimationFolder("FatDemonDeath", 0.034f, false);
	renderer_->CreateAnimationFolder("FatDemonDeathIdle");
	pushHitEffectRenderer(renderer_);

	renderer_->ChangeAnimation("FatDemonIntro");

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
	}
}

void FatDemon::startIntro(float _deltaTime)
{
}

void FatDemon::updateIntro(float _deltaTime)
{
}

void FatDemon::startTransition(float _deltaTime)
{
}

void FatDemon::updateTransition(float _deltaTime)
{
}

void FatDemon::startIdle(float _deltaTime)
{
}

void FatDemon::updateIdle(float _deltaTime)
{
}

void FatDemon::startAttack(float _detlaTime)
{
}

void FatDemon::updateAttack(float _detlaTime)
{
}

void FatDemon::startDeath(float _deltaTime)
{
}

void FatDemon::updateDeath(float _deltaTime)
{
}

void FatDemon::startRelease(float _deltaTime)
{
}

void FatDemon::updateRelease(float _deltaTime)
{
}
