#include "PreCompile.h"
#include "DevilPlatform.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

DevilPlatform::DevilPlatform()
	: renderer_(nullptr)
	, collision_(nullptr)
	, timeCounter_(0.0f)
{

}

DevilPlatform::~DevilPlatform()
{

}

void DevilPlatform::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Platform);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(230.f, 50.f);
	collision_->SetLocationY(80.f);
	
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("DevilPlatform0", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform1", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform2", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform3", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform4", 0.067f);
	renderer_->ChangeAnimation("DevilPlatform0");

	state_.CreateState("Idle", std::bind(&DevilPlatform::startIdle, this, std::placeholders::_1), std::bind(&DevilPlatform::updateIdle, this, std::placeholders::_1));

	state_.ChangeState("Idle");
}

void DevilPlatform::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void DevilPlatform::SetPlatformAppearance(int _num)
{
	_num = _num % 5;

	renderer_->ChangeAnimation("DevilPlatform" + std::to_string(_num));
}

void DevilPlatform::startIdle(float _deltaTime)
{
}

void DevilPlatform::updateIdle(float _deltaTime)
{
}

void DevilPlatform::startMove(float _deltaTime)
{
}

void DevilPlatform::updateMove(float _deltaTime)
{
}

void DevilPlatform::startFall(float _deltaTime)
{
}

void DevilPlatform::updateFall(float _deltaTime)
{
}

