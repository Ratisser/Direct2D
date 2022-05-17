#include "PreCompile.h"
#include "MovePoint.h"

#include <GameEngine\GameEngineCore.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineInput.h>
#include <GameApp\FadeOut.h>

MovePoint::MovePoint()
	: renderer_(nullptr)
	, collision_(nullptr)
	, levelName_("")
{

}

MovePoint::~MovePoint()
{

}

void MovePoint::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->SetTexture("world_platforming_icon_0001.png", true);

	collision_ = CreateTransformComponent<GameEngineCollision>(renderer_);
	collision_->SetCollisionGroup(eCollisionGroup::Map);
	collision_->SetCollisionType(eCollisionType::Rect);

	state_.CreateState(MakeState(MovePoint, Idle));
	state_.CreateState(MakeState(MovePoint, Activate));

	state_ << "Idle";
}

void MovePoint::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void MovePoint::SetDestination(const std::string& _levelName)
{
	levelName_ = _levelName;
}

void MovePoint::startIdle(float _deltaTime)
{
}

void MovePoint::updateIdle(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyDown("Z") && collision_->IsCollideOne(eCollisionGroup::Player))
	{
		state_ << "Activate";
		return;
	}
}

void MovePoint::startActivate(float _deltaTime)
{
	level_->CreateActor<FadeOut>();
}

void MovePoint::updateActivate(float _deltaTime)
{
	if (state_.GetTime() > 0.55f)
	{
		if (levelName_ != "")
		{
			GameEngineCore::ChangeLevel(levelName_);
		}
		state_ << "Idle";
	}
}
