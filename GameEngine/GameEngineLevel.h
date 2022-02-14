#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

class GameEngineActor;
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

public:
	void Update(float _deltaTime);

public:

	template<typename T>
	void CreateActor(int _updateOrder = 0, int _renderOrder = 0)
	{
		GameEngineActor* newActor = new T();

		newActor->SetLevel(this);

		std::list<GameEngineActor*>& actorList = allActors_[_updateOrder];

		actorList.push_back(newActor);
	}


private:
	std::map<int, std::list<GameEngineActor*>> allActors_;

};

