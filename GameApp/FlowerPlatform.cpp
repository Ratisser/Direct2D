#include "PreCompile.h"
#include "FlowerPlatform.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>

#include "DevilChip.h"
#include <GameApp\Player.h>

int FlowerPlatform::VineCount = 0;

FlowerPlatform::FlowerPlatform()
	: renderer_(nullptr)
	, collision_(nullptr)
	, rootTransform_(nullptr)
	, shadowRenderer_(nullptr)
	, propellerRenderer_(nullptr)
	, timeCounter_(0.0f)
	, bVineActivation_(false)
	, vineBackRenderer_(nullptr)
	, vineFrontRenderer_(nullptr)
	, vineCollision_(nullptr)
	, vineTransform_(nullptr)
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


	// vine
	vineTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	vineTransform_->AddLocation(0.0f, -180.f, 0.02f);

	vineBackRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(vineTransform_);
	vineBackRenderer_->CreateAnimationFolder("vineBackBegin", 0.0416f, false);
	vineBackRenderer_->CreateAnimationFolderReverse("vineBackBeginReverse", "vineBackBegin", 0.0416f, false);
	vineBackRenderer_->CreateAnimationFolder("vineBackIdle", 0.0416f);
	vineBackRenderer_->ChangeAnimation("vineBackBegin");
	vineBackRenderer_->Off();

	vineFrontRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(vineTransform_);
	vineFrontRenderer_->SetLocationZ(-0.61f);
	vineFrontRenderer_->CreateAnimationFolder("vineFrontBegin", 0.0416f, false);
	vineFrontRenderer_->CreateAnimationFolderReverse("vineFrontBeginReverse", "vineFrontBegin", 0.0416f, false);
	vineFrontRenderer_->CreateAnimationFolder("vineFrontIdle", 0.0416f);
	vineFrontRenderer_->CreateAnimationFolder("vineFrontAttack", 0.0416f, false);
	vineFrontRenderer_->CreateAnimationFolderReverse("vineFrontAttackReverse", "vineFrontAttack", 0.0416f, false);
	vineFrontRenderer_->Off();

	vineCollision_ = CreateTransformComponent<GameEngineCollision>(vineFrontRenderer_);
	vineCollision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	vineCollision_->SetCollisionType(eCollisionType::Rect);

	vineCollision_->SetScale(0.8f);
	vineCollision_->Off();
	
	vineState_.CreateState(MakeState(FlowerPlatform, VineIdle));
	vineState_.CreateState(MakeState(FlowerPlatform, VineReady));
	vineState_.CreateState(MakeState(FlowerPlatform, VineAttackBegin));
	vineState_.CreateState(MakeState(FlowerPlatform, VineAttackEnd));
	vineState_.CreateState(MakeState(FlowerPlatform, VineClear));
	vineState_.ChangeState("VineIdle");
}

void FlowerPlatform::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (bVineActivation_)
	{
		vineState_.Update(_deltaTime);
	}
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

void FlowerPlatform::ActivateVinePattern()
{
	bVineActivation_ = true;
	vineState_ << "VineIdle";
}

void FlowerPlatform::DeactivateVinePattern()
{
	bVineActivation_ = false;
	vineBackRenderer_->Off();
	vineFrontRenderer_->Off();
	vineCollision_->Off();
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

void FlowerPlatform::startVineIdle(float _deltaTime)
{
}

void FlowerPlatform::updateVineIdle(float _deltaTime)
{
	if (vineState_.GetTime() > 2.0f)
	{
		GameEngineRandom random;
		if (random.RandomBool())
		{
			if (VineCount < 2)
			{
				VineCount++;
				vineState_ << "VineReady";
				return;
			}
		}
		else
		{
			vineState_.SetTime(0.0f);
		}
	}
}

void FlowerPlatform::startVineReady(float _deltaTime)
{
	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 4);
	std::string soundName = "sfx_level_flower_vinehand_grow_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

	vineBackRenderer_->ChangeAnimation("vineBackBegin", true);
	vineBackRenderer_->On();

	vineFrontRenderer_->ChangeAnimation("vineFrontBegin", true);
	vineFrontRenderer_->On();

	vineCollision_->On();
}

void FlowerPlatform::updateVineReady(float _deltaTime)
{
	if (vineBackRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineBackRenderer_->ChangeAnimation("vineBackIdle");
	}

	if (vineFrontRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineFrontRenderer_->ChangeAnimation("vineFrontIdle");
	}

	if (vineState_.GetTime() > 1.0f)
	{
		vineState_ << "VineAttackBegin";
	}
}

void FlowerPlatform::startVineAttackBegin(float _deltaTime)
{
	vineFrontRenderer_->ChangeAnimation("vineFrontAttack");

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 2);
	std::string soundName = "sfx_level_flower_vinehand_grow_continue_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void FlowerPlatform::updateVineAttackBegin(float _deltaTime)
{
	if (vineFrontRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineState_ << "VineAttackEnd";
	}
}

void FlowerPlatform::startVineAttackEnd(float _deltaTime)
{
	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 2);
	std::string soundName = "sfx_level_flower_vinehand_grow_retract_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

	vineFrontRenderer_->ChangeAnimation("vineFrontAttackReverse");
}

void FlowerPlatform::updateVineAttackEnd(float _deltaTime)
{
	if (vineFrontRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineState_ << "VineClear";
	}
}

void FlowerPlatform::startVineClear(float _deltaTime)
{
	vineBackRenderer_->ChangeAnimation("vineBackBeginReverse");
	vineFrontRenderer_->ChangeAnimation("vineFrontBeginReverse");
}

void FlowerPlatform::updateVineClear(float _deltaTime)
{
	if (vineBackRenderer_->GetCurrentAnimation()->IsEnd_ && vineFrontRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineBackRenderer_->Off();
		vineFrontRenderer_->Off();
		vineCollision_->Off();
		VineCount--;
		vineState_ << "VineIdle";
	}
}
