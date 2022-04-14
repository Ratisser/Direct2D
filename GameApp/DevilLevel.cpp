#include "PreCompile.h"
#include "DevilLevel.h"

#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "Player.h"
#include "DevilMap.h"
#include "Devil.h"

DevilLevel::DevilLevel()
	: player_(nullptr)
{

}

DevilLevel::~DevilLevel()
{

}

void DevilLevel::LevelChangeEndEvent()
{
}

void DevilLevel::LevelChangeStartEvent()
{
}

void DevilLevel::LevelStart()
{
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "Devil" / "Background";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "Devil" / "Devil" / "Phase1";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_DevilPhase1_2.wav");

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(400.f, -400.f);

	CreateActor<DevilMap>("DevilMap");

	Devil* devil = CreateActor<Devil>("Devil");
	devil->GetTransform()->SetLocation(750.f, -690.f, 0.8f);

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
}
