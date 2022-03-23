#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "Monster.h"

#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine\GameEngineInput.h>

#include "Map.h"

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
	mainCamera_->GetCamera()->SetLocationZ(-420.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);

	mainCamera_->GetTransform()->SetParent(player->GetTransform());

	GameEngineInput::GetInstance().CreateKey("W", 'W');
	GameEngineInput::GetInstance().CreateKey("A", 'A');
	GameEngineInput::GetInstance().CreateKey("S", 'S');
	GameEngineInput::GetInstance().CreateKey("D", 'D');
}

void PlayLevel::LevelUpdate(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyPress("W"))
	{
		mainCamera_->GetTransform()->AddLocation(0.0f, 100.0f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("A"))
	{
		mainCamera_->GetTransform()->AddLocation(-100.0f * _deltaTime, 0.0f);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("S"))
	{
		mainCamera_->GetTransform()->AddLocation(0.0f, -100.0f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("D"))
	{
		mainCamera_->GetTransform()->AddLocation(100.0f * _deltaTime, 0.0f);
	}
}
