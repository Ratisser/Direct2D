#include "PreCompile.h"
#include "DevilLevel.h"

#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>
#include <GameEngineBase\GameEngineRandom.h>

#include "Player.h"
#include "DevilMap.h"
#include "Devil.h"
#include "Demon.h"
#include "DevilPhaseTwo.h"
#include "DevilPlatform.h"

DevilLevel::DevilLevel()
	: player_(nullptr)
	, devil_(nullptr)
	, devilPhaseTwo_(nullptr)
	, cameraShakeTime_(0.0f)
	, cameraShakeIntensity_(0.0f)
	, timeCounter_(0.0f)
	, currentPhase_(1)
{

}

DevilLevel::~DevilLevel()
{

}

void DevilLevel::LevelChangeEndEvent()
{
	bgmPlayer_->Stop();
}

void DevilLevel::LevelChangeStartEvent()
{
}

void DevilLevel::LevelStart()
{
	// levelState initialize
	{
		levelState_.CreateState("PhaseOne", std::bind(&DevilLevel::startPhaseOne, this, std::placeholders::_1), std::bind(&DevilLevel::updatePhaseOne, this, std::placeholders::_1));
		levelState_.CreateState("EnterPhaseTwo", std::bind(&DevilLevel::startEnterPhaseTwo, this, std::placeholders::_1), std::bind(&DevilLevel::updateEnterPhaseTwo, this, std::placeholders::_1));
		levelState_.CreateState("PhaseTwo", std::bind(&DevilLevel::startPhaseTwo, this, std::placeholders::_1), std::bind(&DevilLevel::updatePhaseTwo, this, std::placeholders::_1));
		levelState_.CreateState("PhaseThree", std::bind(&DevilLevel::startPhaseThree, this, std::placeholders::_1), std::bind(&DevilLevel::updatePhaseThree, this, std::placeholders::_1));
		levelState_.CreateState("PhaseFour", std::bind(&DevilLevel::startPhaseFour, this, std::placeholders::_1), std::bind(&DevilLevel::updatePhaseFour, this, std::placeholders::_1));


		levelState_.ChangeState("PhaseOne");
	}

	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_DevilPhase1_2.wav");

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(400.f, -660.f);
	player_->SetStateCinematic();
	player_->SetCineState("ScaredWait");

	CreateActor<DevilMap>("DevilMap");

	devil_ = CreateActor<Devil>("Devil");

	mainCamera_->GetTransform()->SetLocation(640.f, -360.f, -320.f);
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);

	GameEngineInput::GetInstance().CreateKey("L", 'L');
}

void DevilLevel::LevelUpdate(float _deltaTime)
{
	levelState_.Update(_deltaTime);

	if (!bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Play();
		bgmPlayer_->SetVolume(0.5f);
	}
	
	if (cameraShakeTime_ <= 0.0f)
	{
		mainCameraBackup_->GetCameraComponent()->SetLocation(0.0f, 0.0f, 0.0f);
	}
	else
	{
		cameraShakeTime_ -= _deltaTime;
		GameEngineRandom random;
		float cameraX = random.RandomFloat(0.0f, cameraShakeIntensity_) - cameraShakeIntensity_ * 0.5f;
		float cameraY = random.RandomFloat(0.0f, cameraShakeIntensity_) - cameraShakeIntensity_ * 0.5f;
		mainCameraBackup_->GetCameraComponent()->SetLocationX(cameraX);
		mainCameraBackup_->GetCameraComponent()->SetLocationY(cameraY);
	}


	if (currentPhase_ < 2)
	{
		if (GameEngineInput::GetInstance().IsKeyDown("L"))
		{
			player_->GetTransform()->SetLocationY(-4200.f);
			player_->SetStateNormal();
			dynamic_cast<DevilMap*>(Map::GetCurrentMap())->ChangeCollisionPhaseTwo();
			createActorPhaseTwo();
			levelState_ << "PhaseTwo";
		}
	}
	else
	{
		if (GameEngineInput::GetInstance().IsKeyDown("L"))
		{
			devilPhaseTwo_->SubtractHP(50);
		}
	}

}

void DevilLevel::CameraShake(float _ShakeTime, float _intensity)
{
	cameraShakeTime_ = _ShakeTime;
	cameraShakeIntensity_ = _intensity;
}

void DevilLevel::ChangeStateEnterPhaseTwo()
{
	levelState_ << "EnterPhaseTwo";
}

void DevilLevel::ChangeStatePhaseThree()
{
	levelState_ << "PhaseThree";
}

void DevilLevel::ChangeStatePhaseFour()
{
	levelState_ << "PhaseFour";
}

void DevilLevel::createActorPhaseTwo()
{
	devilPhaseTwo_ = CreateActor<DevilPhaseTwo>("DevilPhaseTwo");
	devilPhaseTwo_->GetTransform()->SetLocation(731.f, PHASE_TWO_BOTTOM, 1.0f);

	for (int i = 0; i < 5; i++)
	{
		DevilPlatform* newDevilPlatform = CreateActor<DevilPlatform>();
		newDevilPlatform->GetTransform()->SetLocation(200.f + 267.f * i, -4400.f, 0.2f);
		newDevilPlatform->GetTransform()->SetScale(0.8f);
		newDevilPlatform->SetPlatformAppearance(i);
		platforms_.push_back(newDevilPlatform);
	}
}

void DevilLevel::startPhaseOne(float _detaTime)
{
}

void DevilLevel::updatePhaseOne(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		mainCamera_->GetTransform()->SetLocationX(640.f + playerPos.x / 8.9f);
	}
}

void DevilLevel::startEnterPhaseTwo(float _deltaTime)
{
	PhaseTwoCamStartLocation_ = mainCamera_->GetTransform()->GetWorldLocation();
	PhaseTwoCamEndLocation_ = { mainCamera_->GetTransform()->GetWorldLocation().x, PHASE_TWO_BOTTOM_CAMERA };

	timeCounter_ = 0;

	createActorPhaseTwo();
}

void DevilLevel::updateEnterPhaseTwo(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > 1.0f)
	{
		mainCameraBackup_->GetTransform()->SetLocation(GameEngineMath::Lerp(PhaseTwoCamStartLocation_, PhaseTwoCamEndLocation_, timeCounter_ - 1.0f, 5.0f));
	}
	
	if (timeCounter_ > 6.0f)
	{
		levelState_ << "PhaseTwo";
		return;
	}
}

void DevilLevel::startPhaseTwo(float _deltaTime)
{
	player_->SetStateCinematic();
	player_->SetCineState("Scared");

	currentPhase_ = 2;
	bgmPlayer_->Stop();
	bgmPlayer_->ChangeSound("MUS_DevilPhase3.wav");

	devilPhaseTwo_->StartIdle();
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_intro_end.wav");

	devil_->Release();
	devil_ = nullptr;

	for (DevilPlatform* p : platforms_)
	{
		p->Activate();
	}
}

void DevilLevel::updatePhaseTwo(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		mainCamera_->GetTransform()->SetLocationX(640.f + (playerPos.x / 8.9f));
		mainCamera_->GetTransform()->SetLocationY(365.f + PHASE_TWO_BOTTOM + (playerPos.y - PHASE_TWO_BOTTOM) / 7.5454f);
	}
}

void DevilLevel::startPhaseThree(float _deltaTime)
{
	currentPhase_ = 3;

	platforms_.front()->FallAndRelease();
	platforms_.back()->FallAndRelease();
	platforms_.pop_front();
	platforms_.pop_back();
}

void DevilLevel::updatePhaseThree(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		mainCamera_->GetTransform()->SetLocationX(640.f + (playerPos.x / 8.9f));
		mainCamera_->GetTransform()->SetLocationY(365.f + PHASE_TWO_BOTTOM + (playerPos.y - PHASE_TWO_BOTTOM) / 7.5454f);
	}
}

void DevilLevel::startPhaseFour(float _deltaTime)
{
	platforms_.front()->FallAndRelease();
	platforms_.back()->FallAndRelease();
	platforms_.pop_front();
	platforms_.pop_back();

	platforms_.front()->SetMoveable(false);
}

void DevilLevel::updatePhaseFour(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		mainCamera_->GetTransform()->SetLocationX(640.f + (playerPos.x / 8.9f));
		mainCamera_->GetTransform()->SetLocationY(365.f + PHASE_TWO_BOTTOM + (playerPos.y - PHASE_TWO_BOTTOM) / 7.5454f);
	}
}
