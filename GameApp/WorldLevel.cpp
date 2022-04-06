#include "PreCompile.h"
#include "WorldLevel.h"

#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "WorldMapPlayer.h"
#include <GameApp\WorldMap.h>

WorldLevel::WorldLevel()
	: player_(nullptr)
	, bgmPlayer_(nullptr)
{

}

WorldLevel::~WorldLevel()
{

}

void WorldLevel::LevelChangeEndEvent()
{
	bgmPlayer_->Stop();
}

void WorldLevel::LevelChangeStartEvent()
{
}

void WorldLevel::LevelStart()
{
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "World" / "Background";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}
	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Image" / "World" / "Cuphead";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory Dir;
		Dir.MoveParent("Direct2D");
		Dir / "Resources" / "Sound" / "World";

		std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance().CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}

	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_InkwellIsleOne.wav");


	mainCamera_->GetCamera()->SetLocationZ(-320.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);

	player_ = CreateActor<WorldMapPlayer>("Player");
	player_->GetTransform()->SetLocation(800.f, -800.f);
	CreateActor<WorldMap>("Map");
}

void WorldLevel::LevelUpdate(float _deltaTime)
{
	mainCamera_->GetTransform()->SetLocation(player_->GetTransform()->GetLocation());

	if (!bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Play();
		bgmPlayer_->SetVolume(0.5f);
	}
}
