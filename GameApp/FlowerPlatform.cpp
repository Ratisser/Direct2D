#include "PreCompile.h"
#include "FlowerPlatform.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>

#include "DevilChip.h"

FlowerPlatform::FlowerPlatform()
	: renderer_(nullptr)
	, collision_(nullptr)
	, rootTransform_(nullptr)
	, bMoveable_(true)
	, bActivate_(false)
	, timeCounter_(0.0f)
{

}

FlowerPlatform::~FlowerPlatform()
{

}

void FlowerPlatform::Start()
{
	rootTransform_ = CreateTransformComponent<GameEngineTransformComponent>();

	collision_ = CreateTransformComponent<GameEngineCollision>(rootTransform_);
	collision_->SetCollisionGroup(eCollisionGroup::Platform);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(240.f, 50.f);
	collision_->SetLocationY(80.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(rootTransform_);
	renderer_->CreateAnimationFolder("DevilPlatform0", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform1", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform2", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform3", 0.067f);
	renderer_->CreateAnimationFolder("DevilPlatform4", 0.067f);
	renderer_->ChangeAnimation("DevilPlatform0");

	state_.CreateState("Wait", std::bind(&FlowerPlatform::startWait, this, std::placeholders::_1), std::bind(&FlowerPlatform::updateWait, this, std::placeholders::_1));
	state_.CreateState("Idle", std::bind(&FlowerPlatform::startIdle, this, std::placeholders::_1), std::bind(&FlowerPlatform::updateIdle, this, std::placeholders::_1));
	state_.CreateState("Move", std::bind(&FlowerPlatform::startMove, this, std::placeholders::_1), std::bind(&FlowerPlatform::updateMove, this, std::placeholders::_1));
	state_.CreateState("RevertMove", std::bind(&FlowerPlatform::startRevertMove, this, std::placeholders::_1), std::bind(&FlowerPlatform::updateRevertMove, this, std::placeholders::_1));
	state_.CreateState("Fall", std::bind(&FlowerPlatform::startFall, this, std::placeholders::_1), std::bind(&FlowerPlatform::updateFall, this, std::placeholders::_1));

	state_.ChangeState("Wait");
}

void FlowerPlatform::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void FlowerPlatform::SetPlatformAppearance(int _num)
{
	_num = _num % 5;
	assert(_num >= 0 && _num < 5);

	renderer_->ChangeAnimation("DevilPlatform" + std::to_string(_num));
}

void FlowerPlatform::SetMoveable(bool _bMoveable)
{
	bMoveable_ = _bMoveable;
	if (!bMoveable_)
	{
		state_ << "RevertMove";
	}
}

void FlowerPlatform::FallAndRelease()
{
	state_ << "Fall";
}

void FlowerPlatform::startWait(float _deltaTime)
{
	timeCounter_ = 0.0f;
}

void FlowerPlatform::updateWait(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (timeCounter_ > 5.0f)
	{
		state_ << "Idle";
		return;
	}
}

void FlowerPlatform::startIdle(float _deltaTime)
{
	timeCounter_ = 0.0f;
}

void FlowerPlatform::updateIdle(float _deltaTime)
{
	if (!bActivate_)
	{
		return;
	}

	timeCounter_ += _deltaTime;

	if (timeCounter_ > ACTION_COOLTIME)
	{
		GameEngineRandom random;
		eAction action = static_cast<eAction>(random.RandomInt(0, eAction::MAX_COUNT - 1));

		if (false == bMoveable_)
		{
			if (action != eAction::CHIP)
			{
				timeCounter_ = 0.0f;
				return;
			}
		}

		switch (action)
		{
		case FlowerPlatform::MOVE:
			state_ << "Move";
			break;
		case FlowerPlatform::STAY:
			timeCounter_ = 0.0f;
			break;
		case FlowerPlatform::REVERT:
			state_ << "RevertMove";
			break;
		case FlowerPlatform::CHIP:
		{
			if (false == DevilChip::IsFalling())
			{
				DevilChip* chip = level_->CreateActor<DevilChip>();
				chip->GetTransform()->SetLocation(transform_->GetWorldLocation() + float4(0.0f, 1000.f, -0.1f));
				DevilChip::FallStart();
			}
		}
		break;
		default:
			break;
		}
	}
}

void FlowerPlatform::startMove(float _deltaTime)
{
	moveStartPosition_ = rootTransform_->GetLocation();
	moveEndPosition_ = moveStartPosition_;

	GameEngineRandom random;
	moveEndPosition_.y = random.RandomFloat(0.0f, MAX_MOVE_HEIGHT);

	timeCounter_ = 0.0f;
}

void FlowerPlatform::updateMove(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	rootTransform_->SetLocation(GameEngineMath::Lerp(moveStartPosition_, moveEndPosition_, timeCounter_, MOVE_DELAY));

	if (timeCounter_ > MOVE_DELAY)
	{
		state_ << "Idle";
	}
}

void FlowerPlatform::startRevertMove(float _deltaTime)
{
	moveStartPosition_ = rootTransform_->GetLocation();
	moveEndPosition_ = float4(0.0f, 0.0f);

	timeCounter_ = 0.0f;
}

void FlowerPlatform::updateRevertMove(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	rootTransform_->SetLocation(GameEngineMath::Lerp(moveStartPosition_, moveEndPosition_, timeCounter_, MOVE_DELAY));

	if (timeCounter_ > MOVE_DELAY)
	{
		state_ << "Idle";
	}
}

void FlowerPlatform::startFall(float _deltaTime)
{
	timeCounter_ = 0.0f;
}

void FlowerPlatform::updateFall(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	transform_->AddLocation(0.0f, -200.f * _deltaTime);
	if (timeCounter_ > 5.0f)
	{
		Release();
	}
}

