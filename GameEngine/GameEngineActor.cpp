#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor()
	: transform_(std::make_unique<GameEngineTransform>())
	, level_(nullptr)
{

}

GameEngineActor::~GameEngineActor()
{
	for (GameEngineComponent* component : allComponents_)
	{
		delete component;
		component = nullptr;
	}
	
	allComponents_.clear();

	for (GameEngineTransformComponent* component : allTransformComponents_)
	{
		delete component;
		component = nullptr;
	}
}

void GameEngineActor::transformUpdate()
{
	transform_->UpdateTransform();
}

void GameEngineActor::updateComponent()
{
	for (GameEngineTransformComponent* tc : allTransformComponents_)
	{
		tc->Update();
		tc->GetTransform()->UpdateTransform();
	}

	for (GameEngineComponent* c : allComponents_)
	{
		c->Update();
	}
}

GameEngineLevel* GameEngineActor::GetLevel()
{
	return level_;
}

void GameEngineActor::SetLevel(GameEngineLevel* _level)
{
	level_ = _level;
}

GameEngineTransform* GameEngineActor::GetTransform()
{
	return transform_.get();
}
