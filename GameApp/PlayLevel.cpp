#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "Monster.h"

#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine\GameEngineInput.h>

PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::LevelChangeEndEvent()
{
}

void PlayLevel::LevelChangeStartEvent()
{
}

void PlayLevel::LevelStart()
{
	CreateActor<Monster>("Monster");
	GameEngineActor* player = CreateActor<Player>("Player");
	mainCamera_->GetTransform()->SetLocation(0.0f, 0.0f, -100.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);

	mainCamera_->GetTransform()->SetParent(player->GetTransform());

	GameEngineInput::GetInstance().CreateKey("W", 'W');
	GameEngineInput::GetInstance().CreateKey("A", 'A');
	GameEngineInput::GetInstance().CreateKey("S", 'S');
	GameEngineInput::GetInstance().CreateKey("D", 'D');
}

void PlayLevel::LevelUpdate(float _deltaTime)
{

}
