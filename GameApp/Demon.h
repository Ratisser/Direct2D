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
	virtual void OnHit() override;

private:
#pragma region State
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
	const int DEMON_HP = 3;
	const float HIT_EFFECT_TIME = 0.1f;
	const float RUN_TIME = 2.0f;
	const float ATTACK_TIME = 4.0f;
	const float4 START_LOCATION = { 720.f, -560.f, 1.0f };
	const float4 LEFT_RUN_END = { -160.f, -560.f, 1.0f };
	const float4 RIGHT_RUN_END = { 1600.f, -560.f, 1.0f };
	const float4 LEFT_ATTACK_START_LOCATION = { -160.f, -660.f, 0.1f };
	const float4 RIGHT_ATTACK_START_LOCATION = { 1600.f, -660.f, 0.1f };

private:
	GameEngineFSM state_;

	GameEngineImageRenderer* renderer_;
	GameEngineCollision* bodyCollision_;

	bool bLeft_;
	float timeCounter_;
	float hitEffectTime_;
};

