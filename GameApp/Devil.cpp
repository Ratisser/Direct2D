#include "PreCompile.h"
#include "Devil.h"

#include <GameEngine\GameEngineImageRenderer.h>

Devil::Devil()
	: renderer_(nullptr)
{

}

Devil::~Devil()
{

}

void Devil::Start()
{
	initRendererAndAnimation();
	initCollision();
	initState();
}

void Devil::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void Devil::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("DevilIdle");

	renderer_->CreateAnimationFolder("RamTransform", 0.034f, false);
	renderer_->CreateAnimationFolderReverse("RamTransformReverse", "RamTransform", 0.034f, false);
	renderer_->CreateAnimationFolder("RamIdle");

	renderer_->ChangeAnimation("DevilIdle");
}

void Devil::initCollision()
{
}

void Devil::initState()
{
	//state_.CreateState("", std::bind(&Devil::start, this, std::placeholders::_1), std::bind(&Devil::update, this, std::placeholders::_1));

	state_.CreateState("Idle", std::bind(&Devil::startIdle, this, std::placeholders::_1), std::bind(&Devil::updateIdle, this, std::placeholders::_1));

	state_.CreateState("RamTransform", std::bind(&Devil::startRamTransform, this, std::placeholders::_1), std::bind(&Devil::updateRamTransform, this, std::placeholders::_1));
	state_.CreateState("RamAttack", std::bind(&Devil::startRamAttack, this, std::placeholders::_1), std::bind(&Devil::updateRamAttack, this, std::placeholders::_1));
	state_.CreateState("RamEnd", std::bind(&Devil::startRamEnd, this, std::placeholders::_1), std::bind(&Devil::updateRamEnd, this, std::placeholders::_1));

	state_.ChangeState("RamTransform");
}


float temp;

void Devil::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("DevilIdle");
	temp = 0;
}

void Devil::updateIdle(float _deltaTime)
{
	temp += _deltaTime;

	if (temp > 2.f)
	{
		state_ << "RamTransform";
		return;
	}
}

void Devil::startRamTransform(float _deltaTime)
{
	renderer_->ChangeAnimation("RamTransform");
	renderer_->SetLocationX(-30.f);
}

void Devil::updateRamTransform(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "RamAttack";
		return;
	}
}

void Devil::startRamAttack(float _deltaTime)
{
	renderer_->ChangeAnimation("RamIdle");
	temp = 0;
}

void Devil::updateRamAttack(float _deltaTime)
{
	temp += _deltaTime;

	if (temp > 2.f)
	{
		state_ << "RamEnd";
		return;
	}
}

void Devil::startRamEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("RamTransformReverse");
}

void Devil::updateRamEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		renderer_->SetLocationX(0.f);
		return;
	}
}

