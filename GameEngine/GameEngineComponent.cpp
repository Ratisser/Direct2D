#include "PreCompile.h"
#include "GameEngineComponent.h"

#include "GameEngineActor.h"

GameEngineComponent::GameEngineComponent(GameEngineActor* _actor)
	: actor_(_actor)
{

}

GameEngineComponent::~GameEngineComponent()
{

}

GameEngineActor* GameEngineComponent::GetActor() const
{
	return actor_;
}
