#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineCollision;
class MonsterBase : public GameEngineActor
{
public:
	MonsterBase(); 
	~MonsterBase();

	MonsterBase(const MonsterBase& _other) = delete; 
	MonsterBase(MonsterBase&& _other) = delete; 

	MonsterBase& operator=(const MonsterBase& _other) = delete;
	MonsterBase& operator=(const MonsterBase&& _other) = delete;
public:
	// GameEngineActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

protected:
	GameEngineCollision* collider_;


};

