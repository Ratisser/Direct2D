#include "PreCompile.h"

#include "TitleLevel.h"
#include "TitleActor.h"

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{

}

void TitleLevel::LevelChangeEndEvent()
{
}

void TitleLevel::LevelChangeStartEvent()
{


}

void TitleLevel::LevelStart()
{
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
	mainCamera_->GetTransform()->SetLocationZ(-300.f);
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir.MoveChild("Resources");
		Dir.MoveChild("Image");
		Dir.MoveChild("Title");

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "Title" / "Cuphead";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	CreateActor<TitleActor>();
}

void TitleLevel::LevelUpdate(float _deltaTime)
{
}