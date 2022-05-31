#include "PreCompile.h"
#include "DevilPhaseTwo.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineCore.h>

#include "GameEngineLevelControlWindow.h"

#include "Axe.h"
#include <GameApp\BombBat.h>
#include <GameApp\DevilLevel.h>
#include <GameApp\DevilImp.h>
#include "FatDemon.h"
#include "DevilTear.h"
#include "Knockout.h"
#include "Player.h"
#include "Explosion.h"
#include <GameApp\FadeOut.h>

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
		headRenderer_->SetAddColor(float4::ZERO);
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("DevilPhase2 Hp : " + std::to_string(hp_));
	}
}

void DevilPhaseTwo::OnHit()
{
	const float4 onHitColor = { 0.1f, 0.2f, 0.3f };
	headRenderer_->SetAddColor(onHitColor);
	hitEffectTime_ = HIT_EFFECT_TIME;

	if (hp_ <= 0)
	{
		leftEyeCollision_->Off();
		rightEyeCollision_->Off();

		state_ << "Death";
	}
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
	headRenderer_->CreateAnimationFolder("DevilImpIntro", 0.034f, false);
	headRenderer_->CreateAnimationFolderReverse("DevilImpIntroReverse", "DevilImpIntro", 0.034f, false);
	headRenderer_->CreateAnimationFolder("DevilSummonImpIdle");
	headRenderer_->CreateAnimationFolder("DevilCrying", 0.034f, false);
	headRenderer_->CreateAnimationFolder("DevilStartCrying", 0.034f, false);
	headRenderer_->CreateAnimationFolder("DevilKnockout");

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
	state_.CreateState("EndSummonImp", std::bind(&DevilPhaseTwo::startEndSummonImp, this, std::placeholders::_1), std::bind(&DevilPhaseTwo::updateEndSummonImp, this, std::placeholders::_1));

	state_.CreateState(MakeState(DevilPhaseTwo, Death));


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
		if (hp_ <= 200)
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

	leftEyeCollision_->SetLocation(LEFT_EYE_LOCATION_PHASE3);
	leftEyeCollision_->SetScale(100.f);
	rightEyeCollision_->SetLocation(RIGHT_EYE_LOCATION_PHASE3);
	rightEyeCollision_->SetScale(100.f);
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
	timeCounter_ = 0.0f;
}

void DevilPhaseTwo::updatePhaseThreeIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > ACTION_DELAY)
	{
		if (hp_ <= 100)
		{

			state_ << "EnterPhaseFour";
			return;
		}

		GameEngineRandom random;

		eAttackStatePhase3 asp3 = static_cast<eAttackStatePhase3>(random.RandomInt(0, static_cast<int>(eAttackStatePhase3::MAX_COUNT) - 1));
		if (static_cast<int>(asp3) == prevState_)
		{
			return;
		}
		//asp3 = eAttackStatePhase3::FAT_DEMON;

		switch (asp3)
		{
		case eAttackStatePhase3::IMP:
			if (DevilImp::GetRefCount() >= 5)
			{
				return;
			}
			state_ << "SummonImp";
			break;
		case eAttackStatePhase3::FAT_DEMON:
			state_ << "SummonFatDemon";
			break;
		default:
			break;
		}
		timeCounter_ = 0.0f;
		prevState_ = static_cast<int>(asp3);
	}
}

void DevilPhaseTwo::startEnterPhaseFour(float _deltaTime)
{
	headRenderer_->ChangeAnimation("Phase2ToPhase3");
	headRenderer_->GetCurrentAnimation()->CurFrame_ = 3;
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_hurt_trans_A_001.wav");

	headTransform_->AddLocation(0.0f, -150.f);

	leftEyeCollision_->SetScale(300.f, 100.f);
	rightEyeCollision_->SetScale(300.f, 100.f);
}

void DevilPhaseTwo::updateEnterPhaseFour(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		DevilLevel* level = dynamic_cast<DevilLevel*>(level_);
		if (nullptr != level)
		{
			level->ChangeStatePhaseFour();
		}

		headRenderer_->ChangeAnimation("DevilCrying");
		headTransform_->AddLocation(0.0f, 150.f);

		state_ << "PhaseFourIdle";
		return;
	}
}

void DevilPhaseTwo::startPhaseFourIdle(float _deltaTime)
{
	headRenderer_->ChangeAnimation("DevilCrying", true);

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 3);
	std::string soundName = "sfx_level_devil_head_devil_cry_idle_00" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

	DevilTear* tear = level_->CreateActor<DevilTear>();

	if (random.RandomInt(0, 2) == 0)
	{
		if (random.RandomInt(0, 1))
		{
			tear->GetTransform()->SetWorldLocationXY(leftEyeCollision_->GetWorldLocation());
		}
		else
		{
			tear->GetTransform()->SetWorldLocationXY(rightEyeCollision_->GetWorldLocation());
		}
	}
}

void DevilPhaseTwo::updatePhaseFourIdle(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "PhaseFourIdle";
		return;
	}
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
	headRenderer_->ChangeAnimation("DevilImpIntro");
	headTransform_->AddLocation(0.0f, -40.f);
}

void DevilPhaseTwo::updateSummonImp(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		headRenderer_->ChangeAnimation("DevilSummonImpIdle");
		GameEngineRandom random;

		for (size_t i = 0; i < 3; i++)
		{
			if (DevilImp::GetRefCount() >= 5)
			{
				break;
			}

			DevilImp* newImp = level_->CreateActor<DevilImp>();
			newImp->GetTransform()->SetLocationZ(random.RandomFloat(0.01f, 0.2f));
		}
	}

	if (state_.GetTime() > 2.0f)
	{
		state_ << "EndSummonImp";
		return;
	}
}

void DevilPhaseTwo::startEndSummonImp(float _deltaTime)
{
	headRenderer_->ChangeAnimation("DevilImpIntroReverse");
}

void DevilPhaseTwo::updateEndSummonImp(float _deltaTime)
{
	if (headRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		headTransform_->AddLocation(0.0f, 40.f);
		state_ << "PhaseThreeIdle";
		return;
	}
}

void DevilPhaseTwo::startSummonFatDemon(float _deltaTime)
{
	if (FatDemon::LeftExist && FatDemon::RightExist)
	{
		return;
	}

	level_->CreateActor<FatDemon>();
}

void DevilPhaseTwo::updateSummonFatDemon(float _deltaTime)
{
	if (state_.GetTime() > 2.0f)
	{
		state_ << "PhaseThreeIdle";
	}
}

void DevilPhaseTwo::startDeath(float _deltaTime)
{
	headTransform_->AddLocation(0.0f, -300.f);
	level_->CreateActor<Knockout>();

	GameEngineSoundManager& sm = GameEngineSoundManager::GetInstance();
	sm.PlaySoundByName("sfx_level_announcer_knockout_0004.wav");
	sm.PlaySoundByName("sfx_level_knockout_boom_01.wav");
	sm.PlaySoundByName("sfx_level_knockout_bell.wav");
	sm.PlaySoundByName("sfx_level_devil_head_devil_dead_loop.wav");

	level_->SetBulletTime(0.2f, 2.0f);

	headRenderer_->ChangeAnimation("DevilKnockout");
	timeCounter_ = 0.0f;
}

void DevilPhaseTwo::updateDeath(float _deltaTime)
{
	static float fadeOutDelay = 4.45f;
	fadeOutDelay -= _deltaTime;
	timeCounter_ += _deltaTime;

	GameEngineRandom random;
	float x = random.RandomFloat(400.f, 1000.f);
	float y = -random.RandomFloat(4000.f, 4600.f);

	if (timeCounter_ > 0.2f)
	{
		Explosion* newExplosion = level_->CreateActor<Explosion>();
		newExplosion->GetTransform()->SetWorldLocationXY(x, y);
		timeCounter_ = 0.0f;
	}

	if (fadeOutDelay < 0.0f)
	{
		fadeOutDelay = 100.f;
		level_->CreateActor<FadeOut>();
	}

	if (state_.GetTime() > 5.f)
	{
		GameEngineCore::ChangeLevel("EndingLevel");
	}
}
