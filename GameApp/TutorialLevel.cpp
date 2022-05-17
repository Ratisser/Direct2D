#include "PreCompile.h"
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "TutorialLevel.h"
#include "Player.h"
#include "TutorialMap.h"

#include "ParryObjectTutorial.h"
#include <GameApp\FadeIn.h>


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
	CreateActor<FadeIn>();
}

void TutorialLevel::LevelStart()
{
	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_Tutorial.wav");

	player_ = CreateActor<Player>("Player");
	player_->GetTransform()->SetLocation(200.f, -400.f, 0.0f);
	//player_->GetTransform()->SetLocation(3890.f, -400.f, 0.0f);


	mainCamera_->GetCameraComponent()->SetLocationZ(-320.f);
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);


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
		bgmPlayer_->SetVolume(0.5f);
	}
}
