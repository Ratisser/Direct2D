#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class DevilImp : public MonsterBase
{
public:
	DevilImp();
	~DevilImp();

	DevilImp(const DevilImp& _other) = delete;
	DevilImp(DevilImp&& _other) = delete;

	DevilImp& operator=(const DevilImp& _other) = delete;
	DevilImp& operator=(const DevilImp&& _other) = delete;

public:
	static int GetRefCount() { return refCounter_; }

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;
	virtual void OnHit() override;

private:
#pragma region State
	void startSpawn(float _deltaTime);
	void updateSpawn(float _deltaTime);
	
	void startFlyUp(float _deltaTime);
	void updateFlyUp(float _deltaTime);

	void startFly(float _deltaTime);
	void updateFly(float _deltaTime);
	
	void startAttack(float _deltaTime);
	void updateAttack(float _deltaTime);

	void startEndAttack(float _deltaTime);
	void updateEndAttack(float _deltaTime);

	void startDeath(float _deltaTime);
	void updateDeath(float _deltaTime);

	void startRelease(float _deltaTime);
	void updateRelease(float _deltaTime);

#pragma endregion

private:
	static int refCounter_;

private:
	const int HP = 2;

	const float FLY_HEIGHT = -3900.f;

	const float FLY_MIN_X = 200.f;
	const float FLY_MAX_X = 1200.f;

	const float SPAWN_MIN_X = 600.f;
	const float SPAWN_MAX_X = 860.f;

	const float SPAWN_MIN_Y = 3900.f; // should be negative
	const float SPAWN_MAX_Y = 4000.f; // should be negative



private:
	GameEngineTransformComponent* childTransform_;
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* spawnRenderer_;
	GameEngineCollision* collision_;

	float4 speed_;

	float accelerationY_;
	float nextAttackDelay_;

	float4 prevLocation_;
	float4 nextLocation_;

	GameEngineFSM state_;
};

