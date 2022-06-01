#include "PreCompile.h"
#include "FlowerLevel.h"

#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "Player.h"
#include "FlowerMap.h"
#include "FadeIn.h"
#include "Ready.h"
#include "Flower.h"
#include "FlowerPlatform.h"

FlowerLevel::FlowerLevel()
	: player_(nullptr)
	, flower_(nullptr)
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

	if (nullptr != flower_)
	{
		flower_->Release();
		flower_ = nullptr;
	}

	bgmPlayer_->Stop();

	for (FlowerPlatform* p : platforms_)
	{
		p->DeactivateVinePattern();
	}
}

void FlowerLevel::LevelChangeStartEvent()
{
	CreateActor<FadeIn>();
	CreateActor<Ready>();

	if (nullptr == player_)
	{
		player_ = CreateActor<Player>("Player");
		player_->GetTransform()->SetWorldLocationXY(246.f, -665.f);
	}

	if (nullptr == flower_)
	{
		flower_ = CreateActor<Flower>("Flower");
		flower_->GetTransform()->SetWorldLocation(1430.f, -740.f, 1.0f);
	}
}

void FlowerLevel::LevelStart()
{
	GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
	mainCameraBackup_->GetTransform()->SetLocation(0.0f, -360.f, -100.f);
	CreateActor<FlowerMap>("FlowerMap");

	{
		FlowerPlatform* fp = CreateActor<FlowerPlatform>();
		fp->GetTransform()->SetWorldLocation(200.f, -530.f, 1.5f);
		platforms_.push_back(fp);

		fp = CreateActor<FlowerPlatform>();
		fp->GetTransform()->SetWorldLocation(450.f, -530.f, 1.5f);
		fp->SetPlatformMoveFactor(-1.0f);
		platforms_.push_back(fp);

		fp = CreateActor<FlowerPlatform>();
		fp->GetTransform()->SetWorldLocation(700.f, -530.f, 1.5f);
		platforms_.push_back(fp);
	}


	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_Flower.wav");
}

void FlowerLevel::LevelUpdate(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();

	if (false == IsFreeCamera())
	{
		float xFactor = 670.f + playerPos.x / 20.265f;
		if (xFactor < 670.f)
		{
			xFactor = 670.f;
		}
		mainCamera_->GetTransform()->SetLocationX(xFactor);
		mainCamera_->GetTransform()->SetLocationY(-808.f + 360.f + (playerPos.y + 808) / 9.1818f);
	}

	if (bgmPlayer_->IsPlaying() == false)
	{
		bgmPlayer_->Play();
	}
}

Player* FlowerLevel::GetPlayer()
{
	if (nullptr == player_)
	{
		GameEngineDebug::MsgBoxError("Player is nullptr");
		return nullptr;
	}
	return player_;
}

Flower* FlowerLevel::GetFlower()
{
	if (nullptr == flower_)
	{
		GameEngineDebug::MsgBoxError("Flower is nullptr");
		return nullptr;
	}
	return flower_;
}

void FlowerLevel::EnterPhase2()
{
	for (FlowerPlatform* p : platforms_)
	{
		p->ActivateVinePattern();
	}
}
