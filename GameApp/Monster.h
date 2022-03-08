#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineCollision;
class Monster : public GameEngineActor
{
public:
	Monster(); 
	~Monster();

	Monster(const Monster& _other) = delete; 
	Monster(Monster&& _other) = delete; 

	Monster& operator=(const Monster& _other) = delete;
	Monster& operator=(const Monster&& _other) = delete;
public:
	// GameEngineActor��(��) ���� ��ӵ�
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineCollision* collider_;


};

