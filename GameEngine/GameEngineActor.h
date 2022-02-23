#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

#include "GameEngineTransform.h"
#include "GameEngineComponent.h"
#include "GameEngineTransformComponent.h"

class GameEngineLevel;
class GameEngineActor : public GameEngineObjectNameBase
{
	friend GameEngineLevel;
public:
	GameEngineActor();
	virtual ~GameEngineActor();

	GameEngineActor(const GameEngineActor& _other) = delete;
	GameEngineActor(GameEngineActor&& _other) = delete;

	GameEngineActor& operator=(const GameEngineActor& _other) = delete;
	GameEngineActor& operator=(const GameEngineActor&& _other) = delete;

public:
	virtual void Start() = 0;
	virtual void Update(float _deltaTime) = 0;

public:
	GameEngineLevel* GetLevel();
	void SetLevel(GameEngineLevel* _level);

	GameEngineTransform* GetTransform();

	template<typename ComponentType>
	ComponentType* CreateComponent();

	template<typename ComponentType>
	ComponentType* CreateTransformComponent(GameEngineTransform* _parent);

protected:
	GameEngineLevel* level_;

private:
	void transformUpdate();
	void updateComponent();

protected:
	std::unique_ptr<GameEngineTransform> transform_;

private:

	std::list<GameEngineComponent*> allComponents_;
	std::list<GameEngineTransformComponent*> allTransformComponents_;
};

template<typename ComponentType>
ComponentType* GameEngineActor::CreateComponent()
{
	GameEngineComponent* newComponent = new ComponentType();

	newComponent->InitComponent(this);
	newComponent->Start();

	allComponents_.push_back(newComponent);

	return dynamic_cast<ComponentType*>(newComponent);
}

template<typename ComponentType>
ComponentType* GameEngineActor::CreateTransformComponent(GameEngineTransform* _parent)
{
	GameEngineTransformComponent* newComponent = new ComponentType();

	newComponent->InitComponent(this);

	if (nullptr != _parent)
	{
		newComponent->GetTransform()->SetParent(_parent);
	}


	newComponent->Start();

	allTransformComponents_.push_back(newComponent);
	return dynamic_cast<ComponentType*>(newComponent);
}