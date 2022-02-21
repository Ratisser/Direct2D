#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

class GameEngineActor;
class GameEngineRenderingComponent;
class GameEngineLevel : public GameEngineObjectNameBase
{
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

public:

	template<typename ActorType>
	void CreateActor(int _updateOrder = 0, int _renderOrder = 0)
	{
		GameEngineActor* newActor = new ActorType();

		newActor->SetLevel(this);

		allActors_[_updateOrder].push_back(newActor);
	}


private:
	std::map<int, std::list<GameEngineActor*>> allActors_;
	std::map<int, std::list<GameEngineRenderingComponent*>> allRenderer_;
};

