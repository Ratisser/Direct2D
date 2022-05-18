#include "PreCompile.h"
#include "WorldLevel.h"

#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "WorldMapPlayer.h"
#include <GameApp\WorldMap.h>
#include "MovePoint.h"
#include <GameApp\FadeIn.h>

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
	CreateActor<FadeIn>();
}

void WorldLevel::LevelStart()
{
	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_InkwellIsleOne.wav");


	mainCamera_->GetCameraComponent()->SetLocationZ(-320.f);
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);

	player_ = CreateActor<WorldMapPlayer>("Player");
	player_->GetTransform()->SetLocation(800.f, -800.f, 3.5f);
	CreateActor<WorldMap>("Map");

	MovePoint* mp = CreateActor<MovePoint>("MovePointDevilLevel");
	mp->GetTransform()->SetWorldLocation({ 2618.f, -1531.f, 4.5f });
	mp->SetDestination("DevilLevel");

	mp = CreateActor<MovePoint>("MovePointFlowerLevel");
	mp->GetTransform()->SetWorldLocation({ 2136.f, -761.f, 4.5f });
	mp->SetDestination("FlowerLevel"); // flower level
}

void WorldLevel::LevelUpdate(float _deltaTime)
{
	float4 pl = player_->GetTransform()->GetLocation();
	mainCamera_->GetTransform()->SetLocation(pl.x, pl.y, -200.f);

	if (!bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Play();
		bgmPlayer_->SetVolume(0.5f);
	}
}
