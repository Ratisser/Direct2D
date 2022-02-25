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
	{
		auto iter = allTransformComponents_.begin();
		auto endIter = allTransformComponents_.end();

		while (iter != endIter)
		{
			GameEngineTransformComponent* tc = *iter;
			if (tc->IsUpdate())
			{
				tc->Update();
				tc->GetTransform()->UpdateTransform();
				iter++;
			}
			else
			{
				delete tc;
				tc = nullptr;

			}
		}
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
