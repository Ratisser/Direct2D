#include "PreCompile.h"
#include "Flower.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineLevel.h>

#include <GameApp\GameEngineLevelControlWindow.h>
#include "GatlingMissile.h"
#include "GatlingSeedBlue.h"
#include "GatlingSeedPink.h"
#include "GatlingSeedPurple.h"

Flower::Flower()
	: renderer_(nullptr)
	, nextState_(0)
	, bodyTransform_(nullptr)
	, headCollision_(nullptr)
	, handCollision_(nullptr)
	, bodyCollision_(nullptr)
	, FaceAttackHighCollision_(nullptr)
	, FaceAttackLowCollision_(nullptr)
	, gatlingLoopSound_(nullptr)
	, timeCounter_(0.0f)
	, gatlingSeedSpawnTime_(0.0f)
{

}

Flower::~Flower()
{

}

void Flower::Start()
{
	SetHP(HP);
	initInput();
	initTransform();
	initRendererAndAnimation();
	initCollision();
	initState();

	gatlingLoopSound_ = std::make_unique<GameEngineSoundPlayer>("sfx_flower_gattling_loop.wav");
}

void Flower::Update(float _deltaTime)
{
	MonsterBase::Update(_deltaTime);

	GameEngineInput& input = GameEngineInput::GetInstance();
	for (int i = 1; i < 5; i++)
	{
		if (input.IsKeyDown(std::to_string(i)))
		{
			nextState_ = i;
		}
	}

	state_.Update(_deltaTime);

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("Flower Hp : " + std::to_string(hp_));
	}
}

void Flower::OnHit()
{
	MonsterBase::OnHit();
}

void Flower::initInput()
{
}

void Flower::initTransform()
{
	bodyTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
}

void Flower::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(bodyTransform_);
	renderer_->CreateAnimationFolder("FlowerIntro", 1.0f, false);
	renderer_->CreateAnimationFolder("FlowerIdle", 0.0416f);
	renderer_->CreateAnimationFolder("FaceAttackHighBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("FaceAttackHighIdle", 0.067f);
	renderer_->CreateAnimationFolder("FaceAttackHighEnd", 0.067f, false);
	renderer_->CreateAnimationFolder("FaceAttackLowBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("FaceAttackLowIdle", 0.067f);
	renderer_->CreateAnimationFolder("FaceAttackLowEnd", 0.067f, false);

	renderer_->CreateAnimationFolder("GatlingBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("GatlingIdle", 0.0416f, true);
	renderer_->CreateAnimationFolder("GatlingEnd", 0.0416f, false);

	renderer_->SetPivot(eImagePivot::BOTTOM_RIGHT);
	renderer_->ChangeAnimation("FlowerIntro");

	pushHitEffectRenderer(renderer_);
	
}

void Flower::initCollision()
{
	headCollision_ = CreateTransformComponent<GameEngineCollision>(bodyTransform_);
	headCollision_->SetScale(250.f, 250.f);
	headCollision_->SetLocation(-300.f, 470.f);
	headCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	headCollision_->SetCollisionType(eCollisionType::Rect);

	handCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	handCollision_->SetScale(120.f);
	handCollision_->SetLocation(916.f, -532.f);
	handCollision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	handCollision_->SetCollisionType(eCollisionType::Rect);
	handCollision_->Off();

	bodyCollision_ = CreateTransformComponent<GameEngineCollision>(bodyTransform_);
	bodyCollision_->SetScale(180.f, 300.f);
	bodyCollision_->SetLocation(-300.f, 200.f);
	bodyCollision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	bodyCollision_->SetCollisionType(eCollisionType::Rect);
	
	handCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	handCollision_->SetScale(120.f);
	handCollision_->SetLocation(916.f, -532.f);
	handCollision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	handCollision_->SetCollisionType(eCollisionType::Rect);
	handCollision_->Off();

	FaceAttackHighCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	FaceAttackHighCollision_->SetScale(900.f, 280.f);
	FaceAttackHighCollision_->SetLocation(625.f, -300.f);
	FaceAttackHighCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	FaceAttackHighCollision_->SetCollisionType(eCollisionType::Rect);
	FaceAttackHighCollision_->Off();

	FaceAttackLowCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	FaceAttackLowCollision_->SetScale(900.f, 280.f);
	FaceAttackLowCollision_->SetLocation(625.f, -630.f);
	FaceAttackLowCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	FaceAttackLowCollision_->SetCollisionType(eCollisionType::Rect);
	FaceAttackLowCollision_->Off();
}

void Flower::initState()
{
	state_.CreateState(MakeState(Flower, Intro));
	state_.CreateState(MakeStateWithEnd(Flower, Idle));
	state_.CreateState(MakeState(Flower, FaceAttackHighBegin));
	state_.CreateState(MakeState(Flower, FaceAttackHighIdle));
	state_.CreateState(MakeState(Flower, FaceAttackHighEnd));
	state_.CreateState(MakeState(Flower, FaceAttackLowBegin));
	state_.CreateState(MakeState(Flower, FaceAttackLowIdle));
	state_.CreateState(MakeState(Flower, FaceAttackLowEnd));
	state_.CreateState(MakeState(Flower, GatlingBegin));
	state_.CreateState(MakeState(Flower, GatlingIdle));
	state_.CreateState(MakeState(Flower, GatlingEnd));

	state_ << "Intro";
}

void Flower::startIntro(float _deltaTime)
{
	
}

void Flower::updateIntro(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_intro_yell.wav");
		renderer_->GetCurrentAnimation()->InterTime_ = 0.0416f;
		state_.SetTime(-10.f);
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
	}
}

void Flower::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerIdle");
}

void Flower::updateIdle(float _deltaTime)
{
	float time = state_.GetTime();

	if (renderer_->GetCurrentAnimation()->CurFrame_ < 5 || renderer_->GetCurrentAnimation()->CurFrame_ > 21)
	{
		handCollision_->On();
	}
	else
	{
		handCollision_->Off();
	}

	if (time > ACTION_DELAY)
	{
		if (hp_ <= 150)
		{
			// phase 2
		}

		GameEngineRandom random;

		eAttackState as = static_cast<eAttackState>(random.RandomInt(0, static_cast<int>(eAttackState::MAX_COUNT) - 1));

		if (0 != nextState_)
		{
			as = static_cast<eAttackState>(nextState_);
		}

		switch (as)
		{
		case Flower::eAttackState::FACE_ATTACK_HIGH:
			state_ << "FaceAttackHighBegin";
			break;
		case Flower::eAttackState::FACE_ATTACK_LOW:
			state_ << "FaceAttackLowBegin";
			break;
		case Flower::eAttackState::GATLING:
			state_ << "GatlingBegin";
			break;
		case Flower::eAttackState::SUMMON_OBEJCT:

			break;
		default:
			break;
		}

		nextState_ = 0;
	}
}

void Flower::endIdle(float _deltaTime)
{
	handCollision_->Off();
}

void Flower::startFaceAttackHighBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackHighBegin");

	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_start.wav");
}

void Flower::updateFaceAttackHighBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "FaceAttackHighIdle";
	}
}

void Flower::startFaceAttackHighIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackHighIdle");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_hold_loop.wav");

	FaceAttackHighCollision_->On();
	headCollision_->Off();
}

void Flower::updateFaceAttackHighIdle(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		FaceAttackHighCollision_->Off();
		headCollision_->On();
		state_ << "FaceAttackHighEnd";
	}
}

void Flower::startFaceAttackHighEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackHighEnd");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_end.wav");
}

void Flower::updateFaceAttackHighEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FlowerIdle");
		renderer_->GetCurrentAnimation()->CurFrame_ = 18;
		state_ << "Idle";
	}
}

void Flower::startFaceAttackLowBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackLowBegin");
	bodyTransform_->AddLocation(0.0f, -110.f);
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_start.wav");
}

void Flower::updateFaceAttackLowBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "FaceAttackLowIdle";
	}
}

void Flower::startFaceAttackLowIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackLowIdle");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_hold_loop.wav");
	FaceAttackLowCollision_->On();
	headCollision_->Off();
}

void Flower::updateFaceAttackLowIdle(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		FaceAttackLowCollision_->Off();
		headCollision_->On();
		state_ << "FaceAttackLowEnd";
	}
}

void Flower::startFaceAttackLowEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackLowEnd");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_end.wav");
}

void Flower::updateFaceAttackLowEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FlowerIdle");
		bodyTransform_->AddLocation(0.0f, 110.f);
		state_ << "Idle";
	}
}

void Flower::startGatlingBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("GatlingBegin");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_gattling_start.wav");
}

void Flower::updateGatlingBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "GatlingIdle";
	}
}

void Flower::startGatlingIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("GatlingIdle");
	gatlingLoopSound_->Play();
}

void Flower::updateGatlingIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	gatlingSeedSpawnTime_ += _deltaTime;
	
	GameEngineRandom random;

	if (gatlingSeedSpawnTime_ > 1.0f)
	{
		gatlingSeedSpawnTime_ = 0.0f;

		float x = random.RandomFloat(200.f, 800.f);

		eSeedColor seedColor = static_cast<eSeedColor>(random.RandomInt(0, 2));

		//seedColor = eSeedColor::BLUE;

		switch (seedColor)
		{
		case Flower::eSeedColor::BLUE:
		{
			GatlingSeedBlue* gs = level_->CreateActor<GatlingSeedBlue>();
			gs->GetTransform()->SetWorldLocation(x, -50.0f, 0.1f);
		}
			break;
		case Flower::eSeedColor::PURPLE:
			break;
		case Flower::eSeedColor::PINK:
		{
			GatlingSeedPink* gs = level_->CreateActor<GatlingSeedPink>();
			gs->GetTransform()->SetWorldLocation(x, -50.0f, 0.1f);
		}
			break;
		default:
			break;
		}

	}

	if (timeCounter_ > 0.1f)
	{
		
		
		bool bFire = random.RandomBool();

		if (bFire)
		{
			GameEngineActor* missile = level_->CreateActor<GatlingMissile>();
			missile->GetTransform()->SetLocation(MISSILE_SPAWN_LOCATION);
			

			timeCounter_ = 0.0f;
		}
		else
		{
			timeCounter_ = 0.05f;
		}
	}



	if (gatlingLoopSound_->IsPlaying() == false)
	{
		gatlingLoopSound_->Play();
		gatlingLoopSound_->SetPosition(500);
	}

	if (state_.GetTime() > 5.0f)
	{
		gatlingLoopSound_->Stop();
		state_ << "GatlingEnd";
	}
}

void Flower::startGatlingEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("GatlingEnd");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_gattling_end.wav");
}

void Flower::updateGatlingEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
	}
}

void Flower::startSummonObjectBegin(float _deltaTime)
{
}

void Flower::updateSummonObjectBegin(float _deltaTime)
{
}

void Flower::startSummonObjectAttack(float _deltaTime)
{
}

void Flower::updateSummonObjectAttack(float _deltaTime)
{
}

void Flower::startSummonObjectEnd(float _deltaTime)
{
}

void Flower::updateSummonObjectEnd(float _deltaTime)
{
}
