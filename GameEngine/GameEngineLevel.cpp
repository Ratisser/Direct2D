#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"

GameEngineLevel::GameEngineLevel()
{

}

GameEngineLevel::~GameEngineLevel()
{

}

void GameEngineLevel::Update(float _deltaTime)
{
	for (std::pair<int, std::list<GameEngineActor*>> pairList : allActors_)
	{
		for (GameEngineActor* actor : pairList.second)
		{
			if (actor->IsUpdate())
			{
				actor->Update(_deltaTime);
			}
		}
	}
}