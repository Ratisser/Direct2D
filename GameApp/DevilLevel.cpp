#include "PreCompile.h"
#include "DevilLevel.h"

#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineTransformComponent.h>


#include "Player.h"
#include "DevilMap.h"

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
		Dir / "Resources" / "Image" / "Devil" / "Devil";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Sound" / "Tutorial";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance().CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(400.f, -400.f);

	CreateActor<DevilMap>("DevilMap");

	mainCamera_->GetTransform()->SetLocationZ(-320.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
}

void DevilLevel::LevelUpdate(float _deltaTime)
{
	//mainCamera_->GetTransform()->SetLocation(player_->GetTransform()->GetLocation());
	mainCamera_->GetTransform()->SetLocationY(-400.f);

	//if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(0.0f, 500.f * _deltaTime);
	//}

	//if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(0.0f, -500.f * _deltaTime);
	//}

	//if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(-500.f * _deltaTime, 0.0f);
	//}

	//if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	//{
	//	mainCamera_->GetTransform()->AddLocation(500.f * _deltaTime, 0.0f);
	//}
}
