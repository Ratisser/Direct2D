#include "PreCompile.h"
#include "DevilPhaseTwo.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineLevel.h>

#include "GameEngineLevelControlWindow.h"

#include "Axe.h"

DevilPhaseTwo::DevilPhaseTwo()
	: neckRenderer_(nullptr)
	, headRenderer_(nullptr)
	, neckTransform_(nullptr)
	, headTransform_(nullptr)
	, timeCounter_(0.0f)
	, bLeft_(false)
{

}

DevilPhaseTwo::~DevilPhaseTwo()
{

}

void DevilPhaseTwo::Start()
{
	SetHP(100);
	initInput();
	initTransform();
	initRendererAndAnimation();
	initCollision();
	initState();
}

void DevilPhaseTwo::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (bLeft_)
	{
		headRenderer_->SetFlip(true, false);
	}
	else
	{
		headRenderer_->SetFlip(false, false);
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("DevilPhase2 Hp : " + std::to_string(hp_));
	}
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
	headRenderer_->CreateAnimationFolder("DevilPhase2Idle", 0.0678f, true);
	headRenderer_->CreateAnimationFolder("SpiralAttack", 0.034f, false);
	headRenderer_->CreateAnimationFolder("SpiralAttackEnd", 0.034f, false);
	headRenderer_->CreateAnimationFolder("BombAttack", 0.034f, false); // summon bomb frame 40

	headRenderer_->ChangeAnimation("DevilPhase2Idle");
}

void DevilPhaseTwo::initCollision()
{
}

void DevilPhaseTwo::initState()
{
	state_.CreateState("Wait", std::bind(&DevilPhaseTwo::startWait, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateWait, this, std::placeholders::_1));
	state_.CreateState("Idle", std::bind(&DevilPhaseTwo::startIdle, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateIdle, this, std::placeholders::_1));

	state_.CreateState("SpiralAttack", std::bind(&DevilPhaseTwo::startSpiralAttack, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSpiralAttack, this, std::placeholders::_1));
	state_.CreateState("SpiralAttackSummonAxe", std::bind(&DevilPhaseTwo::startSpiralAttackSummonAxe, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSpiralAttackSummonAxe, this, std::placeholders::_1));
	state_.CreateState("SpiralAttackEnd", std::bind(&DevilPhaseTwo::startSpiralAttackEnd, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSpiralAttackEnd, this, std::placeholders::_1));
	state_.CreateState("BombAttack", std::bind(&DevilPhaseTwo::startBombAttack, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateBombAttack, this, std::placeholders::_1));

	state_ << "Wait";
}

void DevilPhaseTwo::startWait(float _deltaTime)
{
}

void DevilPhaseTwo::updateWait(float _deltaTime)
{
}

void DevilPhaseTwo::startIdle(float _deltaTime)
{
	neckRenderer_->ChangeAnimation("DevilNeck");
	headRenderer_->ChangeAnimation("DevilPhase2Idle");
	timeCounter_ = 0.0f;
}

void DevilPhaseTwo::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > ACTION_DELAY)
	{
		if (hp_ < 0)
		{
			if (state_.GetCurrentStateName() != "PhaseThree")
			{
				state_ << "PhaseThree";
			}
			return;
		}

		GameEngineRandom random;

		eAttackStatePhase2 asp2 = static_cast<eAttackStatePhase2>(random.RandomInt(0, static_cast<int>(eAttackStatePhase2::MAX_COUNT) - 1));
		if (static_cast<int>(asp2) == prevState_)
		{
			return;
		}
		//asp2 = eAttackStatePhase2::BOMB;

		switch (asp2)
		{
		case eAttackStatePhase2::AXE:
			state_ << "SpiralAttack";
			break;
		case eAttackStatePhase2::BOMB:
			state_ << "BombAttack";
			break;
		default:
			break;
		}
		prevState_ = static_cast<int>(asp2);
	}
}

void DevilPhaseTwo::startIdlePhase3(float _deltaTime)
{
}

void DevilPhaseTwo::updateIdlePhase3(float _deltaTime)
{
}

void DevilPhaseTwo::startSpiralAttack(float _deltaTime)
{
	headRenderer_->ChangeAnimation("SpiralAttack");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_spiral_attack.wav");
}

void DevilPhaseTwo::updateSpiralAttack(float _deltaTime)
{
	if (10 == headRenderer_->GetCurrentAnimation()->CurFrame_)
	{
		state_ << "SpiralAttackSummonAxe";
		return;
	}
}

void DevilPhaseTwo::startSpiralAttackSummonAxe(float _deltaTime)
{
	Axe* newAxe = level_->CreateActor<Axe>();
	newAxe->GetTransform()->SetWorldLocation(733.f, -3970.f);
}

void DevilPhaseTwo::updateSpiralAttackSummonAxe(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "SpiralAttackEnd";
		return;
	}
}

void DevilPhaseTwo::startSpiralAttackEnd(float _deltaTime)
{
	headRenderer_->ChangeAnimation("SpiralAttackEnd");
}

void DevilPhaseTwo::updateSpiralAttackEnd(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		return;
	}
}

void DevilPhaseTwo::startBombAttack(float _deltaTime)
{
	GameEngineRandom random;
	bLeft_ = static_cast<bool>(random.RandomInt(0, 1));

	headRenderer_->ChangeAnimation("BombAttack");
	headTransform_->AddLocation(0.0f, -85.f);

	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_bomb_attack.wav");
}

void DevilPhaseTwo::updateBombAttack(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		headTransform_->AddLocation(0.0f, 85);
		state_ << "Idle";
		return;
	}
}
