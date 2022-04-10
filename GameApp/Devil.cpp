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

	renderer_->ChangeAnimation("DevilIdle");
}

void Devil::initCollision()
{
}

void Devil::initState()
{
	state_.CreateState("Idle", std::bind(&Devil::startIdle, this, std::placeholders::_1), std::bind(&Devil::updateIdle, this, std::placeholders::_1));

	state_.ChangeState("Idle");
}

void Devil::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("DevilIdle");
}

void Devil::updateIdle(float _deltaTime)
{
}

