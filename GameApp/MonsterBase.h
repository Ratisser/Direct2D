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
	virtual void OnHit() = 0;
public:
	void SetHP(int _hp);
	void SubtractHP(int _damage);

	int GetHP() { return hp_; }

protected:
	int hp_;

};

