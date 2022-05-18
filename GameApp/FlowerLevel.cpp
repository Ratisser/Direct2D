#include "PreCompile.h"
#include "FlowerLevel.h"

#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "Player.h"
#include "FlowerMap.h"
#include "FadeIn.h"
#include "Ready.h"

FlowerLevel::FlowerLevel()
	: player_(nullptr)
	, bgmPlayer_(nullptr)
{

}

FlowerLevel::~FlowerLevel()
{

}

void FlowerLevel::LevelChangeEndEvent()
{
	if (nullptr != player_)
	{
		player_->Release();
		player_ = nullptr;
	}

	bgmPlayer_->Stop();
}

void FlowerLevel::LevelChangeStartEvent()
{
	CreateActor<FadeIn>();
	CreateActor<Ready>();

	if (nullptr == player_)
	{
		player_ = CreateActor<Player>("Player");
		player_->GetTransform()->SetWorldLocationXY(206.f, -665.f);
	}
}

void FlowerLevel::LevelStart()
{
	GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
	mainCameraBackup_->GetTransform()->SetLocation(0.0f, -360.f, -100.f);
	CreateActor<FlowerMap>("FlowerMap");

	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_Flower.wav");
}

void FlowerLevel::LevelUpdate(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		mainCamera_->GetTransform()->SetLocationX(640.f + playerPos.x / 14.06f);
		mainCamera_->GetTransform()->SetLocationY(-808.f + 360.f + (playerPos.y + 808) / 9.1818f);
	}

	if (bgmPlayer_->IsPlaying() == false)
	{
		bgmPlayer_->Play();
	}
}

Player* FlowerLevel::GetPlayer()
{
	return player_;
}
