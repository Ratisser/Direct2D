#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"

GameEngineLevel::GameEngineLevel()
{

}

GameEngineLevel::~GameEngineLevel()
{
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		for (GameEngineActor* actor : pair.second)
		{
			if (nullptr != actor)
			{
				delete actor;
				actor = nullptr;
			}
		}
	}
}

void GameEngineLevel::ActorUpdate(float _deltaTime)
{
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		for (GameEngineActor* actor : pair.second)
		{
			if (actor->IsUpdate())
			{
				actor->Update(_deltaTime);
			}
		}
	}
}