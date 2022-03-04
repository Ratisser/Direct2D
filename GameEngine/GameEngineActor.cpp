#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor()
	: transform_(std::make_unique<GameEngineTransformComponent>())
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

void GameEngineActor::updateComponent(float _deltaTime)
{
	{
		auto iter = allTransformComponents_.begin();
		auto endIter = allTransformComponents_.end();

		while (iter != endIter)
		{
			GameEngineTransformComponent* tc = *iter;
			if (tc->IsUpdate())
			{
				tc->Update(_deltaTime);
				tc->UpdateTransform();
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
		c->Update(_deltaTime);
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

GameEngineTransformComponent* GameEngineActor::GetTransform()
{
	return transform_.get();
}
