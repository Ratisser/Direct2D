#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "ParryObjectBase.h"

class GameEngineImageRenderer;
class OrbFire : public ParryObjectBase
{
public:
	OrbFire();
	~OrbFire();

	OrbFire(const OrbFire& _other) = delete;
	OrbFire(OrbFire&& _other) = delete;

	OrbFire& operator=(const OrbFire& _other) = delete;
	OrbFire& operator=(const OrbFire&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void Initialize(const float4& _startPosition, float _moveDelay, bool _bParryable = false);

protected:
	virtual void onParry() override;

private:
	void startSummoning(float _deltaTime);
	void updateSummoning(float _deltaTime);

	void startSummonComplete(float _deltaTime);
	void updateSummonComplete(float _deltaTime);

	void startMove(float _deltaTime);
	void updateMove(float _deltaTime);

	void startDeath(float _deltaTime);
	void updateDeath(float _deltaTime);

private:
	const int MAX_SPARK_EFFECT_COUNT = 4;
	const float SPEED = 600.f;


private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* summonEffectRenderer_;
	GameEngineImageRenderer* summonSparkRenderer_;

	float4 direction_;

	int sparkEffectCount_;

	float timeCounter_;

	float moveDelay_;
};

