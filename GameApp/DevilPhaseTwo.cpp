#include "PreCompile.h"
#include "DevilPhaseTwo.h"

#include <GameEngine\GameEngineImageRenderer.h>

DevilPhaseTwo::DevilPhaseTwo()
	: neckRenderer_(nullptr)
	, headRenderer_(nullptr)
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
}

void DevilPhaseTwo::initRendererAndAnimation()
{
	neckRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	neckRenderer_->CreateAnimationFolder("DevilNeck");
	neckRenderer_->ChangeAnimation("DevilNeck");

	headRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
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

}
