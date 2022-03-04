#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

#include "GameEngineComponent.h"
#include "GameEngineTransformComponent.h"

class GameEngineActor : public GameEngineObjectNameBase
{
	friend class GameEngineLevel;
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

	GameEngineTransformComponent* GetTransform();

	template<typename ComponentType>
	ComponentType* CreateComponent();

	template<typename ComponentType>
	ComponentType* CreateTransformComponent(GameEngineTransformComponent* _parent);

protected:
	GameEngineLevel* level_;

private:
	void transformUpdate();
	void updateComponent(float _deltaTime);

protected:
	std::unique_ptr<GameEngineTransformComponent> transform_;

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
ComponentType* GameEngineActor::CreateTransformComponent(GameEngineTransformComponent* _parent)
{
	GameEngineTransformComponent* newComponent = new ComponentType();

	newComponent->InitComponent(this);

	if (nullptr != _parent)
	{
		newComponent->SetParent(_parent);
	}


	newComponent->Start();

	allTransformComponents_.push_back(newComponent);
	return dynamic_cast<ComponentType*>(newComponent);
}