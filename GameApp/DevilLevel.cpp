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

DevilLevel::DevilLevel()
	: player_(nullptr)
	, devil_(nullptr)
	, cameraShakeTime_(0.0f)
	, cameraShakeIntensity_(0.0f)
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
	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_DevilPhase1_2.wav");

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(400.f, -400.f);

	CreateActor<DevilMap>("DevilMap");

	devil_ = CreateActor<Devil>("Devil");

	mainCamera_->GetTransform()->SetLocation(640.f, -360.f, -320.f);
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
}

void DevilLevel::LevelUpdate(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		mainCamera_->GetTransform()->SetLocationX(640.f + playerPos.x / 8.9f);
	}

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
}

void DevilLevel::CameraShake(float _ShakeTime, float _intensity)
{
	cameraShakeTime_ = _ShakeTime;
	cameraShakeIntensity_ = _intensity;
}
