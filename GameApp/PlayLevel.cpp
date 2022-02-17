#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"

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
	CreateActor<Player>();
}

void PlayLevel::LevelUpdate(float _deltaTime)
{
}
