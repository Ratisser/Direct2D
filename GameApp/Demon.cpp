#include "PreCompile.h"
#include "Demon.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

Demon::Demon()
	: renderer_(nullptr)
	, bodyCollision_(nullptr)
	, bLeft_(false)
{
}

Demon::~Demon()
{

}

void Demon::Start()
{
	state_.CreateState("Intro", std::bind(&Demon::startIntro, this, std::placeholders::_1), std::bind(&Demon::updateIntro, this, std::placeholders::_1));

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Intro", 0.034f, false);
	renderer_->CreateAnimationFolder("Intro2", 0.034f, false);
	renderer_->CreateAnimationFolder("Intro3", 0.034f, false);
	renderer_->CreateAnimationFolder("Attack");
	renderer_->CreateAnimationFolder("Jump", 0.034f, false);

	bodyCollision_ = CreateTransformComponent<GameEngineCollision>();
	bodyCollision_->SetCollisionType(eCollisionType::Rect);
	bodyCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	bodyCollision_->SetLocationY(-50.f);
	bodyCollision_->SetScale(96.f);
	bodyCollision_->Off();
}

void Demon::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (bLeft_)
	{
		renderer_->SetFlip(false, false);
	}
	else
	{
		renderer_->SetFlip(true, false);
	}
}

void Demon::startIntro(float _deltaTime)
{
}

void Demon::updateIntro(float _deltaTime)
{
}

void Demon::startJump(float _deltaTime)
{
}

void Demon::updateJump(float _deltaTime)
{
}

void Demon::startRun(float _deltaTime)
{
}

void Demon::updateRun(float _deltaTime)
{
}

void Demon::startAttack(float _deltaTime)
{
}

void Demon::updateAttack(float _deltaTime)
{
}

void Demon::startDie(float _deltaTime)
{
}

void Demon::updateDie(float _deltaTime)
{
}

void Demon::startEnd(float _deltaTime)
{
}

void Demon::updateEnd(float _deltaTime)
{
}
