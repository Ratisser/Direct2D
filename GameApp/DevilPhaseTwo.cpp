#include "PreCompile.h"
#include "DevilPhaseTwo.h"

#include <GameEngine\GameEngineImageRenderer.h>

DevilPhaseTwo::DevilPhaseTwo()
	: neckRenderer_(nullptr)
	, headRenderer_(nullptr)
	, neckTransform_(nullptr)
	, headTransform_(nullptr)
	, timeCounter_(0.0f)
{

}

DevilPhaseTwo::~DevilPhaseTwo()
{

}

void DevilPhaseTwo::Start()
{
	initInput();
	initTransform();
	initRendererAndAnimation();
	initCollision();
	initState();
}

void DevilPhaseTwo::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void DevilPhaseTwo::OnHit()
{
}

void DevilPhaseTwo::initInput()
{
}

void DevilPhaseTwo::initTransform()
{
	neckTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	neckTransform_->SetLocationX(30.f);
	neckTransform_->SetLocationY(-100.f);
	neckTransform_->SetLocationZ(0.01f);
	neckTransform_->SetScale(0.8f);
	
	headTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	headTransform_->SetLocationY(40.f);
	headTransform_->SetScale(0.75f);
}

void DevilPhaseTwo::initRendererAndAnimation()
{
	neckRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(neckTransform_);
	neckRenderer_->CreateAnimationFolder("DevilNeck", 0.1f);
	neckRenderer_->ChangeAnimation("DevilNeck");

	headRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(headTransform_);
	headRenderer_->CreateAnimationFolder("DevilPhase2Idle", 0.034f, true);

	headRenderer_->ChangeAnimation("DevilPhase2Idle");
}

void DevilPhaseTwo::initCollision()
{
}

void DevilPhaseTwo::initState()
{
	state_.CreateState("Idle", std::bind(&DevilPhaseTwo::startIdle, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateIdle, this, std::placeholders::_1));

	state_ << "Idle";
}

void DevilPhaseTwo::startIdle(float _deltaTime)
{
	neckRenderer_->ChangeAnimation("DevilNeck");
	headRenderer_->ChangeAnimation("DevilPhase2Idle");
}

void DevilPhaseTwo::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > 2.f)
	{
		if (hp_ < 0)
		{
			if (state_.GetCurrentStateName() != "PhaseTwo")
			{
				state_ << "PhaseTwo";
			}
			return;
		}
	}
}

void DevilPhaseTwo::startAxe(float _deltaTime)
{
}

void DevilPhaseTwo::updateAxe(float _deltaTime)
{
}

void DevilPhaseTwo::startSummonAxe(float _deltaTime)
{
}

void DevilPhaseTwo::updateSummonAxe(float _deltaTime)
{
}

void DevilPhaseTwo::startEndAxe(float _deltaTime)
{
}

void DevilPhaseTwo::updateEndAxe(float _deltaTime)
{
}
