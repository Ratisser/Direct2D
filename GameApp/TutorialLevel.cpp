#include "PreCompile.h"
#include "TutorialLevel.h"

#include "Player.h"
#include "TutorialMap.h"

TutorialLevel::TutorialLevel()
	: player_(nullptr)
{

}

TutorialLevel::~TutorialLevel()
{

}

void TutorialLevel::LevelChangeEndEvent()
{
}

void TutorialLevel::LevelChangeStartEvent()
{
}

void TutorialLevel::LevelStart()
{
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "Tutorial" / "Background";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "Tutorial" / "Guides";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(200.f, -400.f, 0.0f);


	mainCamera_->GetCamera()->SetLocationZ(-320.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);


	CreateActor<TutorialMap>("Map");
}

void TutorialLevel::LevelUpdate(float _deltaTime)
{
	mainCamera_->GetTransform()->SetLocation(player_->GetTransform()->GetLocation());
	mainCamera_->GetTransform()->SetLocationY(-475.f);
}
