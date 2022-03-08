#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "Monster.h"

#include <GameEngine/GameEngineCameraComponent.h>

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
	CreateActor<Player>("Player");
	CreateActor<Monster>("Monster");
	mainCamera_->GetTransform()->SetLocation(0.0f, 0.0f, -100.f);
	mainCamera_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
}

void PlayLevel::LevelUpdate(float _deltaTime)
{
}
