#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "ParryObjectBase.h"

class GameEngineImageRenderer;
class OrbBubble : public ParryObjectBase
{
public:
	OrbBubble(); 
	~OrbBubble();

	OrbBubble(const OrbBubble& _other) = delete; 
	OrbBubble(OrbBubble&& _other) = delete; 

	OrbBubble& operator=(const OrbBubble& _other) = delete;
	OrbBubble& operator=(const OrbBubble&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void Initialize(const float4& _startPosition, const float4& _direction, bool _bParryable = false);

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

	GameEngineTransformComponent* left_;
	GameEngineTransformComponent* top_;
	GameEngineTransformComponent* right_;
	GameEngineTransformComponent* bottom_;

	float4 direction_;

	int sparkEffectCount_;

	float timeCounter_;
};

