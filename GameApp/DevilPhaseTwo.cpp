#include "PreCompile.h"
#include "DevilPhaseTwo.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineCollision.h>

#include "GameEngineLevelControlWindow.h"

#include "Axe.h"
#include <GameApp\BombBat.h>
#include <GameApp\DevilLevel.h>

DevilPhaseTwo::DevilPhaseTwo()
	: neckRenderer_(nullptr)
	, headRenderer_(nullptr)
	, neckTransform_(nullptr)
	, headTransform_(nullptr)
	, leftEyeCollision_(nullptr)
	, rightEyeCollision_(nullptr)
	, timeCounter_(0.0f)
	, hitEffectTime_(0.0f)
	, prevState_(0)
	, bLeft_(false)
	, bBombSpawned_(false)
{

}

DevilPhaseTwo::~DevilPhaseTwo()
{

}

void DevilPhaseTwo::Start()
{
	SetHP(DEVIL_HP);
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

	if (hitEffectTime_ > 0.0f)
	{
		hitEffectTime_ -= _deltaTime;
	}
	else
	{
		headRenderer_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("DevilPhase2 Hp : " + std::to_string(hp_));
	}
}

void DevilPhaseTwo::OnHit()
{
	const float4 onHitColor = { 0.6f, 0.8f, 1.0f, 1.0f };
	headRenderer_->SetColor(onHitColor);
	hitEffectTime_ = HIT_EFFECT_TIME;
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

	headRenderer_->CreateAnimationFolder("Phase2ToPhase3", 0.067f, false);
	headRenderer_->CreateAnimationFolder("Phase3Idle", 0.034f);

	headRenderer_->ChangeAnimation("DevilPhase2Idle");
}

void DevilPhaseTwo::initCollision()
{
	leftEyeCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	leftEyeCollision_->SetCollisionGroup(eCollisionGroup::MonsterHitBox);
	leftEyeCollision_->SetCollisionType(eCollisionType::Rect);
	leftEyeCollision_->SetScale(100.f, 150.f);
	leftEyeCollision_->SetLocation(LEFT_EYE_LOCATION);

	rightEyeCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	rightEyeCollision_->SetCollisionGroup(eCollisionGroup::MonsterHitBox);
	rightEyeCollision_->SetCollisionType(eCollisionType::Rect);
	rightEyeCollision_->SetScale(100.f, 150.f);
	rightEyeCollision_->SetLocation(RIGHT_EYE_LOCATION);
}

void DevilPhaseTwo::initState()
{
	state_.CreateState("Wait", std::bind(&DevilPhaseTwo::startWait, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateWait, this, std::placeholders::_1));
	state_.CreateState("Idle", std::bind(&DevilPhaseTwo::startIdle, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateIdle, this, std::placeholders::_1));

	state_.CreateState("SpiralAttack", std::bind(&DevilPhaseTwo::startSpiralAttack, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSpiralAttack, this, std::placeholders::_1));
	state_.CreateState("SpiralAttackSummonAxe", std::bind(&DevilPhaseTwo::startSpiralAttackSummonAxe, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSpiralAttackSummonAxe, this, std::placeholders::_1));
	state_.CreateState("SpiralAttackEnd", std::bind(&DevilPhaseTwo::startSpiralAttackEnd, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSpiralAttackEnd, this, std::placeholders::_1));
	state_.CreateState("BombAttack", std::bind(&DevilPhaseTwo::startBombAttack, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateBombAttack, this, std::placeholders::_1));

	state_.CreateState("EnterPhaseThree", std::bind(&DevilPhaseTwo::startEnterPhaseThree, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateEnterPhaseThree, this, std::placeholders::_1));
	state_.CreateState("PhaseThreeIdle", std::bind(&DevilPhaseTwo::startPhaseThreeIdle, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updatePhaseThreeIdle, this, std::placeholders::_1));

	state_.CreateState("EnterPhaseFour", std::bind(&DevilPhaseTwo::startEnterPhaseFour, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateEnterPhaseFour, this, std::placeholders::_1));
	state_.CreateState("PhaseFourIdle", std::bind(&DevilPhaseTwo::startPhaseFourIdle, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updatePhaseFourIdle, this, std::placeholders::_1));

	state_.CreateState("SummonImp", std::bind(&DevilPhaseTwo::startSummonImp, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSummonImp, this, std::placeholders::_1));
	state_.CreateState("SummonFatDemon", std::bind(&DevilPhaseTwo::startSummonFatDemon, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateSummonFatDemon, this, std::placeholders::_1));
	state_.CreateState("ReleaseFatDemon", std::bind(&DevilPhaseTwo::startReleaseFatDemon, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateReleaseFatDemon, this, std::placeholders::_1));
	state_.CreateState("EndFatDemon", std::bind(&DevilPhaseTwo::startEndFatDemon, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateEndFatDemon, this, std::placeholders::_1));

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
		if (hp_ < 200)
		{

			state_ << "EnterPhaseThree";
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

void DevilPhaseTwo::startEnterPhaseThree(float _deltaTime)
{
	headRenderer_->ChangeAnimation("Phase2ToPhase3");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_hurt_trans_A_003.wav");

	headTransform_->AddLocation(0.0f, -150.f);
}

void DevilPhaseTwo::updateEnterPhaseThree(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		DevilLevel* level = dynamic_cast<DevilLevel*>(level_);
		if (nullptr != level)
		{
			level->ChangeStatePhaseThree();
		}

		headRenderer_->ChangeAnimation("Phase3Idle");
		headTransform_->AddLocation(0.0f, 150.f);

		state_ << "PhaseThreeIdle";
		return;
	}
}

void DevilPhaseTwo::startPhaseThreeIdle(float _deltaTime)
{
	headRenderer_->ChangeAnimation("Phase3Idle");
}

void DevilPhaseTwo::updatePhaseThreeIdle(float _deltaTime)
{
}

void DevilPhaseTwo::startEnterPhaseFour(float _deltaTime)
{
}

void DevilPhaseTwo::updateEnterPhaseFour(float _deltaTime)
{
}

void DevilPhaseTwo::startPhaseFourIdle(float _deltaTime)
{
}

void DevilPhaseTwo::updatePhaseFourIdle(float _deltaTime)
{
}

void DevilPhaseTwo::startSpiralAttack(float _deltaTime)
{
	headRenderer_->ChangeAnimation("SpiralAttack");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_spiral_attack.wav");

	timeCounter_ = 0.0f;
}

void DevilPhaseTwo::updateSpiralAttack(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	leftEyeCollision_->SetLocation(GameEngineMath::Lerp(LEFT_EYE_LOCATION, CENTER_EYE_LOCATION, timeCounter_, 0.6f));
	rightEyeCollision_->SetLocation(GameEngineMath::Lerp(RIGHT_EYE_LOCATION, CENTER_EYE_LOCATION, timeCounter_, 0.6f));

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
	timeCounter_ += _deltaTime;
	leftEyeCollision_->SetLocation(GameEngineMath::Lerp(LEFT_EYE_LOCATION, CENTER_EYE_LOCATION, timeCounter_, 0.3f));
	rightEyeCollision_->SetLocation(GameEngineMath::Lerp(RIGHT_EYE_LOCATION, CENTER_EYE_LOCATION, timeCounter_, 0.3f));

	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "SpiralAttackEnd";
		return;
	}
}

void DevilPhaseTwo::startSpiralAttackEnd(float _deltaTime)
{
	headRenderer_->ChangeAnimation("SpiralAttackEnd");
	timeCounter_ = 0.0f;
}

void DevilPhaseTwo::updateSpiralAttackEnd(float _deltaTime)
{
	if (21 < headRenderer_->GetCurrentAnimation()->CurFrame_)
	{
		timeCounter_ += _deltaTime;
		leftEyeCollision_->SetLocation(GameEngineMath::Lerp(CENTER_EYE_LOCATION, LEFT_EYE_LOCATION, timeCounter_, 0.3f));
		rightEyeCollision_->SetLocation(GameEngineMath::Lerp(CENTER_EYE_LOCATION, RIGHT_EYE_LOCATION, timeCounter_, 0.3f));
	}


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
	bBombSpawned_ = false;

	headRenderer_->ChangeAnimation("BombAttack");
	headTransform_->AddLocation(0.0f, -85.f);

	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_bomb_attack.wav");
}

void DevilPhaseTwo::updateBombAttack(float _deltaTime)
{
	if (!bBombSpawned_ && headRenderer_->GetCurrentAnimation()->CurFrame_ == 40)
	{
		bBombSpawned_ = true;

		if (bLeft_)
		{
			BombBat* newBomb = level_->CreateActor<BombBat>();
			newBomb->GetTransform()->SetLocation(LEFT_BOMB_SPAWN_LOCATION);
			newBomb->Initialize(bLeft_);
		}
		else
		{
			BombBat* newBomb = level_->CreateActor<BombBat>();
			newBomb->GetTransform()->SetLocation(RIGHT_BOMB_SPAWN_LOCATION);
			newBomb->Initialize(bLeft_);
		}
	}

	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		headTransform_->AddLocation(0.0f, 85);
		state_ << "Idle";
		return;
	}
}

void DevilPhaseTwo::startSummonImp(float _deltaTime)
{
}

void DevilPhaseTwo::updateSummonImp(float _deltaTime)
{
}

void DevilPhaseTwo::startSummonFatDemon(float _deltaTime)
{
}

void DevilPhaseTwo::updateSummonFatDemon(float _deltaTime)
{
}

void DevilPhaseTwo::startReleaseFatDemon(float _deltaTime)
{
}

void DevilPhaseTwo::updateReleaseFatDemon(float _deltaTime)
{
}

void DevilPhaseTwo::startEndFatDemon(float _deltaTime)
{
}

void DevilPhaseTwo::updateEndFatDemon(float _deltaTime)
{
}
