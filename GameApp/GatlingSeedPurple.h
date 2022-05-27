#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class GatlingSeedPurple : public MonsterBase
{
public:
	GatlingSeedPurple();
	~GatlingSeedPurple();
	GatlingSeedPurple(const GatlingSeedPurple& _other) = delete;
	GatlingSeedPurple(GatlingSeedPurple&& _other) = delete;
	GatlingSeedPurple& operator=(const GatlingSeedPurple& _other) = delete;
	GatlingSeedPurple& operator=(const GatlingSeedPurple&& _other) = delete;

public:
	// MonsterBase��(��) ���� ��ӵ�
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
	const int HP = 4;
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