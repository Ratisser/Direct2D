#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class GatlingSeedBlue : public MonsterBase
{
public:
	GatlingSeedBlue(); 
	~GatlingSeedBlue();
	GatlingSeedBlue(const GatlingSeedBlue& _other) = delete; 
	GatlingSeedBlue(GatlingSeedBlue&& _other) = delete; 
	GatlingSeedBlue& operator=(const GatlingSeedBlue& _other) = delete;
	GatlingSeedBlue& operator=(const GatlingSeedBlue&& _other) = delete;

public:
	// MonsterBase을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	virtual void OnHit() override;

private:
	void startFall(float _deltaTime);
	void updateFall(float _deltaTime);

	void startLanding(float _deltaTime);
	void updateLanding(float _deltaTime);

	void startGrowUp(float _deltaTime);
	void updateGrowUp(float _deltaTime);

	void startSpawn(float _deltaTime);
	void updateSpawn(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startDeath(float _deltaTime);
	void updateDeath(float _deltaTime);

private:
	const int HP = 2;
	const float FALL_SPEED = -200.f;
	const float MOVE_SPEED = 200.f;

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* seedRenderer_;
	GameEngineImageRenderer* vineRenderer_;

	GameEngineTransformComponent* vineTransform_;

	GameEngineCollision* collision_;

	float4 direction_;

	float timeCounter_;
};
