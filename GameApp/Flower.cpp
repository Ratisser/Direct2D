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
#include <GameApp\Boomerang.h>
#include "Acorn.h"
#include <GameApp\Pollen.h>

Flower::Flower()
	: renderer_(nullptr)
	, createObjectEffect_(nullptr)
	, nextState_(0)
	, bodyTransform_(nullptr)
	, headCollision_(nullptr)
	, handCollision_(nullptr)
	, bodyCollision_(nullptr)
	, vineCollision_(nullptr)
	, faceAttackHighCollision_(nullptr)
	, faceAttackLowCollision_(nullptr)
	, gatlingLoopSound_(nullptr)
	, timeCounter_(0.0f)
	, gatlingSeedSpawnTime_(0.0f)
	, bPurpleSeedSpawn_(false)
	, vineRenderer_(nullptr)
	, vineTransform_(nullptr)
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

	if (GameEngineInput::GetInstance().IsKeyDown("L"))
	{
		SubtractHP(50);
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

	if (hp_ <= 0)
	{
		state_ << "KnockOut";
	}
}

void Flower::initInput()
{
	GameEngineInput::GetInstance().CreateKey("L", 'L');
}

void Flower::initTransform()
{
	bodyTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	vineTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	vineTransform_->SetLocation(950.f, -720.f, 1.01f);
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

	renderer_->CreateAnimationFolder("CreateObjectBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("CreateObjectIdle", 0.0416f);
	renderer_->CreateAnimationFolder("CreateObjectRelease", 0.0416f, false);
	renderer_->CreateAnimationFolder("CreateObjectReleaseIdle", 0.0678f);
	renderer_->CreateAnimationFolderReverse("CreateObjectReleaseReverse", "CreateObjectRelease", 0.0416f, false);
	renderer_->CreateAnimationFolder("CreateObjectEnd", 0.0416f, false);

	renderer_->CreateAnimationFolder("FlowerFinalIntro1", 0.0416f, false);
	renderer_->CreateAnimationFolder("FlowerFinalIntro2", 0.0416f, true);
	renderer_->CreateAnimationFolder("FlowerFinalIntro3", 0.0416f, false);
	renderer_->CreateAnimationFolder("FlowerFinalIntro4", 0.0416f, true);
	renderer_->CreateAnimationFolder("FlowerFinalIntro5", 0.0416f, false);

	renderer_->CreateAnimationFolder("FlowerFinal_Idle", 0.0416f);

	renderer_->CreateAnimationFolder("FlowerFinalSpitBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("FlowerFinalSpitEnd", 0.0416f, false);

	renderer_->CreateAnimationFolder("FlowerDeath", 0.0416f, true);

	renderer_->SetPivot(eImagePivot::BOTTOM_RIGHT);
	renderer_->ChangeAnimation("FlowerIntro");

	pushHitEffectRenderer(renderer_);
	
	vineRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(vineTransform_);
	vineRenderer_->CreateAnimationFolder("Ivy_Main", 0.0416f, false);
	vineRenderer_->CreateAnimationFolder("Ivy_MainIdle", 0.1f);
	vineRenderer_->SetPivot(eImagePivot::BOTTOM_RIGHT);
	vineRenderer_->ChangeAnimation("Ivy_Main");
	vineRenderer_->Off();

	createObjectEffect_ = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	createObjectEffect_->SetPivot(eImagePivot::CENTER);
	createObjectEffect_->CreateAnimationFolder("CreateObjectEffect", 0.033f, false);
	createObjectEffect_->ChangeAnimation("CreateObjectEffect");
	createObjectEffect_->Off();
	createObjectEffect_->SetWorldLocation(1000.f, -360.f, -0.5f);
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

	faceAttackHighCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	faceAttackHighCollision_->SetScale(950.f, 280.f);
	faceAttackHighCollision_->SetLocation(625.f, -300.f);
	faceAttackHighCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	faceAttackHighCollision_->SetCollisionType(eCollisionType::Rect);
	faceAttackHighCollision_->Off();

	faceAttackLowCollision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	faceAttackLowCollision_->SetScale(950.f, 280.f);
	faceAttackLowCollision_->SetLocation(625.f, -630.f);
	faceAttackLowCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	faceAttackLowCollision_->SetCollisionType(eCollisionType::Rect);
	faceAttackLowCollision_->Off();

	vineCollision_ = CreateTransformComponent<GameEngineCollision>(vineRenderer_);
	vineCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	vineCollision_->SetCollisionType(eCollisionType::Rect);
	vineCollision_->SetLocationY(-100.0f / 235.f);
	vineCollision_->Off();
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

	state_.CreateState(MakeState(Flower, BoomerangBegin));
	state_.CreateState(MakeState(Flower, BoomerangAttack));
	state_.CreateState(MakeState(Flower, BoomerangEnd));
	state_.CreateState(MakeState(Flower, AcornBegin));
	state_.CreateState(MakeState(Flower, AcornAttack));
	state_.CreateState(MakeState(Flower, AcornEnd));

	// Phase2
	state_.CreateState(MakeState(Flower, Phase2Intro));
	state_.CreateState(MakeState(Flower, Phase2Intro2));
	state_.CreateState(MakeState(Flower, Phase2Intro3));
	state_.CreateState(MakeState(Flower, Phase2Idle));
	state_.CreateState(MakeState(Flower, SpitBegin));
	state_.CreateState(MakeState(Flower, SpitEnd));
	state_.CreateState(MakeState(Flower, KnockOut));

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
			state_ << "Phase2Intro";
			return;
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
		case Flower::eAttackState::SUMMON_OBJECT:
			state_ << "BoomerangBegin";
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

	faceAttackHighCollision_->On();
	headCollision_->Off();
}

void Flower::updateFaceAttackHighIdle(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		faceAttackHighCollision_->Off();
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
	faceAttackLowCollision_->On();
	headCollision_->Off();
}

void Flower::updateFaceAttackLowIdle(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		faceAttackLowCollision_->Off();
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

	bPurpleSeedSpawn_ = false;
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
		{
			if (!bPurpleSeedSpawn_)
			{
				GatlingSeedPurple* gs = level_->CreateActor<GatlingSeedPurple>();
				gs->GetTransform()->SetWorldLocation(x, -50.0f, 0.1f);
				bPurpleSeedSpawn_ = true;
			}
			else
			{
				GatlingSeedBlue* gs = level_->CreateActor<GatlingSeedBlue>();
				gs->GetTransform()->SetWorldLocation(x, -50.0f, 0.1f);
			}
		}
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

void Flower::startBoomerangBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("CreateObjectBegin");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_pot_hands_start.wav");
}

void Flower::updateBoomerangBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("CreateObjectIdle");
	}

	if (state_.GetTime() > 0.5f)
	{
		state_ << "BoomerangAttack";
	}
}

void Flower::startBoomerangAttack(float _deltaTime)
{
	renderer_->ChangeAnimation("CreateObjectRelease");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_pot_hands_open.wav");
}

void Flower::updateBoomerangAttack(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		level_->CreateActor<Boomerang>();
		renderer_->ChangeAnimation("CreateObjectReleaseIdle");
		createObjectEffect_->ChangeAnimation("CreateObjectEffect", true);
		createObjectEffect_->On();
	}

	if (createObjectEffect_->GetCurrentAnimation()->IsEnd_)
	{
		createObjectEffect_->Off();
	}

	if (state_.GetTime() > 1.0f)
	{
		state_ << "BoomerangEnd";
	}
}

void Flower::startBoomerangEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("CreateObjectReleaseReverse");
}

void Flower::updateBoomerangEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "AcornBegin";
	}
}

void Flower::startAcornBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("CreateObjectIdle");
}

void Flower::updateAcornBegin(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		state_ << "AcornAttack";
	}
}

void Flower::startAcornAttack(float _deltaTime)
{
	renderer_->ChangeAnimation("CreateObjectRelease");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_pot_hands_open.wav");
}

void Flower::updateAcornAttack(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		level_->CreateActor<Acorn>();
		level_->CreateActor<Acorn>();
		level_->CreateActor<Acorn>();
		renderer_->ChangeAnimation("CreateObjectReleaseIdle");
		createObjectEffect_->ChangeAnimation("CreateObjectEffect", true);
		createObjectEffect_->On();
	}

	if (createObjectEffect_->GetCurrentAnimation()->IsEnd_)
	{
		createObjectEffect_->Off();
	}

	if (state_.GetTime() > 2.5f)
	{
		state_ << "AcornEnd";
	}
}

void Flower::startAcornEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("CreateObjectEnd");
}

void Flower::updateAcornEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
	}
}

void Flower::startPhase2Intro(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerFinalIntro1");

	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_man_phaseOneTwo_transition.wav");
}

void Flower::updatePhase2Intro(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FlowerFinalIntro2");
	}

	if (state_.GetTime() > 1.0f)
	{
		state_ << "Phase2Intro2";
	}
}

void Flower::startPhase2Intro2(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerFinalIntro3");
}

void Flower::updatePhase2Intro2(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->CurFrame_ == 7)
	{
		renderer_->GetCurrentAnimation()->InterTime_ = 0.5f;
	}
	else
	{
		renderer_->GetCurrentAnimation()->InterTime_ = 0.0416f;
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FlowerFinalIntro4");
		vineRenderer_->On();
		vineCollision_->On();
	}

	if (vineRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineRenderer_->ChangeAnimation("Ivy_MainIdle");
		GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_intro_vinegrow.wav");
	}

	if (state_.GetTime() > 2.0f)
	{
		state_ << "Phase2Intro3";
	}
}

void Flower::startPhase2Intro3(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerFinalIntro5");
	headCollision_->AddLocation(0.0f, -100.f);
}

void Flower::updatePhase2Intro3(float _deltaTime)
{
	if (vineRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		vineRenderer_->ChangeAnimation("Ivy_MainIdle");
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Phase2Idle";
	}
}

void Flower::startPhase2Idle(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerFinal_Idle");
}

void Flower::updatePhase2Idle(float _deltaTime)
{
	if (state_.GetTime() > 5.0f)
	{
		state_ << "SpitBegin";
	}
}

void Flower::startSpitBegin(float _deltaTime)
{
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_phase2_spit_projectile.wav");
	renderer_->ChangeAnimation("FlowerFinalSpitBegin");
}

void Flower::updateSpitBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "SpitEnd";
	}
}

void Flower::startSpitEnd(float _deltaTime)
{
	level_->CreateActor<Pollen>();
	renderer_->ChangeAnimation("FlowerFinalSpitEnd");
}

void Flower::updateSpitEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Phase2Idle";
	}
}

void Flower::startKnockOut(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerDeath");
}

void Flower::updateKnockOut(float _deltaTime)
{
}
