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
	//mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Perspective);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
}

void DevilLevel::LevelUpdate(float _deltaTime)
{
	float4 camPos = mainCamera_->GetTransform()->GetWorldLocation();
	float4 playerPos = player_->GetTransform()->GetWorldLocation();
	if (camPos.x >= 650.f && camPos.x <= 790.f)
	{
		if (camPos.x - playerPos.x > 200.f && camPos.x != 650.f)
		{
			mainCamera_->GetTransform()->AddLocation(-100.f * _deltaTime, 0.0f);
		}
		else if (camPos.x - playerPos.x < -200.f && camPos.x != 790.f)
		{
			mainCamera_->GetTransform()->AddLocation(100.f * _deltaTime, 0.0f);
		}
	}

	if (camPos.x < 650.f)
	{
		mainCamera_->GetTransform()->SetLocationX(650.f);
	}
	else if (camPos.x > 790.f)
	{
		mainCamera_->GetTransform()->SetLocationX(790.f);
	}

	float4 location = mainCamera_->GetTransform()->GetLocation();
	mainCamera_->GetTransform()->SetLocationX(static_cast<float>(location.ix()));


	//
	//if (GameEngineInput::GetInstance().IsKeyPress("W"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(0.0f, 1000.f * _deltaTime);
	//}

	//if (GameEngineInput::GetInstance().IsKeyPress("S"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(0.0f, -1000 * _deltaTime);
	//}

	//if (GameEngineInput::GetInstance().IsKeyPress("A"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(-1000 * _deltaTime, 0.0f);
	//}

	//if (GameEngineInput::GetInstance().IsKeyPress("D"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(1000 * _deltaTime, 0.0f);
	//}

	//if (GameEngineInput::GetInstance().IsKeyDown("W"))
	//{
	//	player_->GetTransform()->SetLocation(400.f, -400.f);
	//}

	if (!bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Play();
		bgmPlayer_->SetVolume(0.5f);
	}
}
