#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

#include "GameEngineCamera.h"
#include "GameEngineCameraComponent.h"

class GameEngineActor;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineCamera;
class GameEngineCameraComponent;
class GameEngineCore;
class GameEngineLevel : public GameEngineObjectNameBase
{
	friend GameEngineCore;
	friend GameEngineRenderer;
	friend GameEngineCollision;
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
	virtual void LevelUpdateAfter(float _deltaTime);

public:
	void ActorUpdate(float _deltaTime);
	void Render();
	void Release(float _deltaTime);

	void SwitchFreeCamera();
	bool IsFreeCamera();

	GameEngineCamera* GetMainCameraActor();
	GameEngineCamera* GetFreeCameraActor();
	GameEngineCameraComponent* GetMainCameraComponent();

	std::list<GameEngineCollision*>& GetCollisionGroup(int _group);

	void SetBulletTime(float _ratio, float _effectTime);

public:
	static bool PostProcess_;

public:
	template<typename ActorType>
	ActorType* CreateActor(int _updateOrder = 0);

	template<typename ActorType>
	ActorType* CreateActor(const std::string& _name, int _updateOrder = 0);

	template<typename LevelType>
	LevelType* GetLevel();

private:
	void init();
	void pushRenderer(GameEngineRenderer* _renderingComponent);
	void pushCollision(int _group, GameEngineCollision* _collision);
	void popCollision(int _group, GameEngineCollision* _collision);
	void popRenderer(GameEngineRenderer* _renderingComponent);

	void levelChangeStartActorEvent();
	void levelChangeEndActorEvent();

	void postProcess();

protected:
	GameEngineCamera* mainCamera_;
	GameEngineCamera* uiCamera_;
	GameEngineCamera* freeCamera_;
	GameEngineCamera* mainCameraBackup_;

private:
	std::map<int, std::list<GameEngineActor*>> allActors_;
	std::map<int, std::list<GameEngineCollision*>> allCollisions_;
	std::list<GameEngineRenderer*> allRenderer_;

private:
	float bulletTimeRatio_;
	float bulletTime_;
};



















template<typename ActorType>
ActorType* GameEngineLevel::CreateActor(int _updateOrder)
{
	GameEngineActor* newActor = new ActorType();

	newActor->SetName("unnamed");
	newActor->SetLevel(this);
	newActor->Start();

	allActors_[_updateOrder].push_back(newActor);

#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString("Create Actor unnamed\n");
#endif // _DEBUG


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

#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString("Create Actor " + _name + "\n");
#endif // _DEBUG

	return dynamic_cast<ActorType*>(newActor);
}

template<typename LevelType>
inline LevelType* GameEngineLevel::GetLevel()
{
	LevelType* level = dynamic_cast<LevelType*>(this);
	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("Down casting failed");
		return nullptr;
	}
	return level;
}