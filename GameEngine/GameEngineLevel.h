#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>
#include <set>

#include "GameEngineCamera.h"

class GameEngineActor;
class GameEngineRenderer;
class GameEngineCamera;
class GameEngineCore;
class GameEngineLevel : public GameEngineObjectNameBase
{
	friend GameEngineCore;
	friend GameEngineRenderer;
	friend GameEngineActor;
public:
	GameEngineLevel();
	~GameEngineLevel();

	GameEngineLevel(const GameEngineLevel& _other) = delete;
	GameEngineLevel(GameEngineLevel&& _other) = delete;

	GameEngineLevel& operator=(const GameEngineLevel& _other) = delete;
	GameEngineLevel& operator=(const GameEngineLevel&& _other) = delete;

public:
	virtual void LevelChangeEndEvent() = 0;
	virtual void LevelChangeStartEvent() = 0;
	virtual void LevelStart() = 0;
	virtual void LevelUpdate(float _deltaTime) = 0;

public:
	void ActorUpdate(float _deltaTime);
	void Render();
	void Release(float _deltaTime);

	GameEngineCamera* GetMainCameraActor();
	GameEngineCameraComponent* GetMainCameraComponent();

public:
	template<typename ActorType>
	ActorType* CreateActor(int _updateOrder = 0);

	template<typename ActorType>
	ActorType* CreateActor(const std::string& _name, int _updateOrder = 0);

private:
	void init();
	void pushRenderer(GameEngineRenderer* _renderingComponent);
	void popRenderer(GameEngineRenderer* _renderingComponent);

protected:
	GameEngineCamera* mainCamera_;

private:
	std::map<int, std::list<GameEngineActor*>> allActors_;
	std::set<GameEngineRenderer*> allRenderer_;
};




template<typename ActorType>
ActorType* GameEngineLevel::CreateActor(int _updateOrder)
{
	GameEngineActor* newActor = new ActorType();

	newActor->SetName("unnamed");
	newActor->SetLevel(this);
	newActor->Start();

	allActors_[_updateOrder].push_back(newActor);
	return dynamic_cast<ActorType*>(newActor);
}

template<typename ActorType>
ActorType* GameEngineLevel::CreateActor(const std::string& _name, int _updateOrder)
{
	GameEngineActor* newActor = new ActorType();

	newActor->SetName(_name);
	newActor->SetLevel(this);
	newActor->Start();

	allActors_[_updateOrder].push_back(newActor);
	return dynamic_cast<ActorType*>(newActor);
}
