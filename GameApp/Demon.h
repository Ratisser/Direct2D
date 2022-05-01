#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class Demon : public MonsterBase
{
public:
	Demon(); 
	~Demon();
	Demon(const Demon& _other) = delete;
	Demon(Demon&& _other) = delete; 
	Demon& operator=(const Demon& _other) = delete;
	Demon& operator=(const Demon&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
#pragma region State
	void startIntro(float _deltaTime);
	void updateIntro(float _deltaTime);

	void startJump(float _deltaTime);
	void updateJump(float _deltaTime);

	void startRun(float _deltaTime);
	void updateRun(float _deltaTime);

	void startAttack(float _deltaTime);
	void updateAttack(float _deltaTime);

	void startDie(float _deltaTime);
	void updateDie(float _deltaTime);

	void startEnd(float _deltaTime);
	void updateEnd(float _deltaTime);

#pragma endregion


private:
	GameEngineFSM state_;

	GameEngineImageRenderer* renderer_;
	GameEngineCollision* bodyCollision_;

	bool bLeft_;
};

