#include "PreCompile.h"
#include "FlowerPlatform.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>

#include "DevilChip.h"
#include <GameApp\Player.h>

FlowerPlatform::FlowerPlatform()
	: renderer_(nullptr)
	, collision_(nullptr)
	, rootTransform_(nullptr)
	, shadowRenderer_(nullptr)
	, propellerRenderer_(nullptr)
	, timeCounter_(0.0f)
{

}

FlowerPlatform::~FlowerPlatform()
{

}

void FlowerPlatform::Start()
{
	rootTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	rootTransform_->SetLocationZ(-0.01f);

	collision_ = CreateTransformComponent<GameEngineCollision>(rootTransform_);
	collision_->SetCollisionGroup(eCollisionGroup::Platform);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(150.f, 30.f);
	collision_->SetLocationY(40.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(rootTransform_);
	renderer_->CreateAnimationFolder("FlowerPlatform0", 0.067f);
	renderer_->CreateAnimationFolder("FlowerPlatform1", 0.067f);
	renderer_->CreateAnimationFolder("FlowerPlatform2", 0.067f);
	renderer_->ChangeAnimation("FlowerPlatform0");

	propellerRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(rootTransform_);
	propellerRenderer_->CreateAnimationFolder("FlowerPlatformPropellor", 0.067f);
	propellerRenderer_->ChangeAnimation("FlowerPlatformPropellor");
	propellerRenderer_->SetPivot(eImagePivot::CENTER);
	propellerRenderer_->SetLocationX(5.0f);

	shadowRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(transform_);
	shadowRenderer_->CreateAnimationFolder("PlatformShadow", 0.067f);
	shadowRenderer_->ChangeAnimation("PlatformShadow");
	shadowRenderer_->SetPivot(eImagePivot::CENTER);
	shadowRenderer_->SetLocationY(-120.f);

	state_.CreateState(MakeState(FlowerPlatform, Wait));
	state_.CreateState(MakeState(FlowerPlatform, Idle));
	state_.CreateState(MakeState(FlowerPlatform, Stop));

	state_.ChangeState("Wait");
}

void FlowerPlatform::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void FlowerPlatform::SetPlatformAppearance(int _num)
{
	_num = _num % 3;
	assert(_num >= 0 && _num < 3);

	renderer_->ChangeAnimation("FlowerPlatform" + std::to_string(_num));
}

void FlowerPlatform::SetPlatformMoveFactor(float _sinfValue)
{
	timeCounter_ = _sinfValue;
}



void FlowerPlatform::startWait(float _deltaTime)
{
}

void FlowerPlatform::updateWait(float _deltaTime)
{
	if (state_.GetTime() > 2.0f)
	{
		state_ << "Idle";
		return;
	}
}

void FlowerPlatform::startIdle(float _deltaTime)
{
}

void FlowerPlatform::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	rootTransform_->SetLocationY(sinf(timeCounter_) * MAX_MOVE_HEIGHT);

	GameEngineCollision* col = collision_->IsCollideOne(eCollisionGroup::Player);
	if (nullptr != col)
	{
		Player* player = dynamic_cast<Player*>(col->GetActor());

		if (nullptr != player)
		{
			if (player->GetNormalState() != "Jump" && player->GetNormalState() != "Parry")
			{
				state_ << "Stop";
				return;
			}
		}
	}
}

void FlowerPlatform::startStop(float _deltaTime)
{
	prevLocation_ = rootTransform_->GetLocation();
	nextLocation_ = prevLocation_;

	timeCounter_ = -0.5f;

	nextLocation_.y = sinf(-1.f) * MAX_MOVE_HEIGHT;
}

void FlowerPlatform::updateStop(float _deltaTime)
{
	rootTransform_->SetLocation(GameEngineMath::Lerp(prevLocation_, nextLocation_, state_.GetTime(), 0.1f));

	if (nullptr == collision_->IsCollideOne(eCollisionGroup::Player))
	{
		state_ << "Idle";
		return;
	}
}
