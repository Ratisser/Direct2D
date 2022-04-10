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

public:
	void SubtractHP(int _damage);

protected:
	int hp_;

};

