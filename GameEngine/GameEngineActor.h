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
	virtual void ReleaseEvent() {};

public:
	template<typename ComponentType>
	ComponentType* CreateComponent();

	template<typename ComponentType>
	ComponentType* CreateTransformComponent();

	template<typename ComponentType>
	ComponentType* CreateTransformComponent(GameEngineTransformComponent* _parent);

public:
	void Release(float _delay = 0.0f);
	void ReleaseComponent();
	void ReleaseUpdate(float _deltaTime);


	GameEngineLevel* GetLevel();
	void SetLevel(GameEngineLevel* _level);

	GameEngineTransformComponent* GetTransform();

	bool GetBulletTimeEffect() const { return bBulletTimeEffect_; }
	void SetBulletTimeEffect(bool _bBulletTimeEffect) { bBulletTimeEffect_ = _bBulletTimeEffect; }

protected:
	virtual void levelChangeEndEvent() {}
	virtual void levelChangeStartEvent() {}

protected:
	GameEngineLevel* level_;
	GameEngineTransformComponent* transform_;

private:
	void transformUpdate();
	void updateComponent(float _deltaTime);

private:
	std::list<GameEngineComponent*> allComponents_;
	std::list<GameEngineTransformComponent*> allTransformComponents_;

	bool bDestroyed_;
	bool bBulletTimeEffect_;
	float aliveTime_;
};













template<typename ComponentType>
ComponentType* GameEngineActor::CreateComponent()
{
	GameEngineComponent* newComponent = new ComponentType();

	newComponent->InitComponent(this);
	newComponent->Start();

	allComponents_.push_back(newComponent);

#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString("Create component\n");
#endif // _DEBUG

	return dynamic_cast<ComponentType*>(newComponent);
}

template<typename ComponentType>
ComponentType* GameEngineActor::CreateTransformComponent()
{
	GameEngineTransformComponent* newComponent = new ComponentType();

	newComponent->InitComponent(this);
	newComponent->SetParent(this->transform_);
	newComponent->Start();

	allTransformComponents_.push_back(newComponent);

#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString("Create transformcomponent\n");
#endif // _DEBUG

	return dynamic_cast<ComponentType*>(newComponent);
}

template<typename ComponentType>
ComponentType* GameEngineActor::CreateTransformComponent(GameEngineTransformComponent* _parent)
{
	GameEngineTransformComponent* newComponent = new ComponentType();

	newComponent->InitComponent(this);
	newComponent->SetParent(_parent);
	newComponent->Start();

	allTransformComponents_.push_back(newComponent);

#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString("Create transformcomponent has parent\n");
#endif // _DEBUG

	return dynamic_cast<ComponentType*>(newComponent);
}

