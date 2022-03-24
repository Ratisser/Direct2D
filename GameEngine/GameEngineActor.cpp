#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderer.h"

GameEngineActor::GameEngineActor()
	: level_(nullptr)
	, bDestroyed_(false)
	, aliveTime_(-1.f)
{
	transform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
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
			else if (tc->IsDeath())
			{
				delete tc;
				tc = nullptr;
				iter = allTransformComponents_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	for (GameEngineComponent* c : allComponents_)
	{
		c->Update(_deltaTime);
	}
}

void GameEngineActor::Release(float _delay)
{
	if (0.0f >= _delay)
	{
		Death();
	}
	else
	{
		bDestroyed_ = true;
		aliveTime_ = _delay;
	}
}

void GameEngineActor::ReleaseComponent()
{
	{
		std::list<GameEngineComponent*>::iterator startIter = allComponents_.begin();
		std::list<GameEngineComponent*>::iterator endIter = allComponents_.end();

		while (startIter != endIter)
		{
			GameEngineComponent* component = *startIter;
			if (component->IsDeath())
			{
				delete component;
				component = nullptr;
				startIter = allComponents_.erase(startIter);
			}
			else
			{
				++startIter;
			}
		}
	}


	// 자식 컴포넌트들을 같이 해제해 주기 위해 해제 준비를 한다.
	{
		for (GameEngineTransformComponent* transformComponent : allTransformComponents_)
		{
			if (transformComponent->IsDeath())
			{
				transformComponent->ReleaseReady();

				GameEngineRenderer* renderer = dynamic_cast<GameEngineRenderer*>(transformComponent);
				if (nullptr != renderer)
				{
					renderer->Death();
					level_->popRenderer(renderer);
				}
			}
		}

		std::list<GameEngineTransformComponent*>::iterator startIter = allTransformComponents_.begin();
		std::list<GameEngineTransformComponent*>::iterator endIter = allTransformComponents_.end();

		while (startIter != endIter)
		{
			GameEngineTransformComponent* component = *startIter;
			if (component->IsDeath())
			{
				delete component;
				component = nullptr;
				startIter = allTransformComponents_.erase(startIter);
			}
			else
			{
				++startIter;
			}
		}
	}
}

void GameEngineActor::ReleaseUpdate(float _deltaTime)
{
	if (false == bDestroyed_)
	{
		return;
	}

	aliveTime_ -= _deltaTime;

	if (0.0f >= aliveTime_)
	{
		ReleaseEvent();
		Death();

		for (GameEngineComponent* component : allComponents_)
		{
			component->Death();
		}

		for (GameEngineTransformComponent* transformComponent : allTransformComponents_)
		{
			transformComponent->Death();
		}
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
	return transform_;
}
