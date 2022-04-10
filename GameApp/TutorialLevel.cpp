#include "PreCompile.h"
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "TutorialLevel.h"
#include "Player.h"
#include "TutorialMap.h"

#include "ParryObjectTutorial.h"


TutorialLevel::TutorialLevel()
	: player_(nullptr)
	, bgmPlayer_(nullptr)
{

}

TutorialLevel::~TutorialLevel()
{

}

void TutorialLevel::LevelChangeEndEvent()
{
	bgmPlayer_->Stop();
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

	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_Tutorial.wav");

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(200.f, -400.f, 0.0f);
	//player_->GetTransform()->SetLocation(3890.f, -400.f, 0.0f);


	mainCamera_->GetCamera()->SetLocationZ(-320.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);


	CreateActor<TutorialMap>("Map");

	ParryObjectTutorial* parry = CreateActor<ParryObjectTutorial>();
	parry->GetTransform()->SetLocation(3890.f, -370.f, 0.0f);

	parry = CreateActor<ParryObjectTutorial>();
	parry->GetTransform()->SetLocation(4042.f, -370.f, 0.0f);

	parry = CreateActor<ParryObjectTutorial>();
	parry->GetTransform()->SetLocation(4192.f, -370.f, 0.0f);
}

void TutorialLevel::LevelUpdate(float _deltaTime)
{
	float4 pl = player_->GetTransform()->GetLocation();
	mainCamera_->GetTransform()->SetLocation(pl.x, pl.y);
	mainCamera_->GetTransform()->SetLocationY(-475.f);

	if (!bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Play();
		//bgmPlayer_->SetVolume(0.5f);
	}
}
