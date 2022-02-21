#include "PreCompile.h"
#include "GameEngineComponent.h"

#include "GameEngineActor.h"

GameEngineComponent::GameEngineComponent()
{

}

GameEngineComponent::~GameEngineComponent()
{

}

void GameEngineComponent::InitComponent(GameEngineActor* _actor)
{
	actor_ = _actor;
	level_ = actor_->GetLevel();
}

GameEngineActor* GameEngineComponent::GetActor() const
{
	return actor_;
}

GameEngineLevel* GameEngineComponent::GetLevel() const
{
	return level_;
}
