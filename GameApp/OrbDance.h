#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "ParryObjectBase.h"

class GameEngineCollision;
class GameEngineImageRenderer;
class OrbDance : public ParryObjectBase
{
public:
	struct NonParryOrb
	{
		GameEngineImageRenderer* Renderer_;
		GameEngineImageRenderer* SummonEffectRenderer_;
		GameEngineImageRenderer* SummonSparkRenderer_;
		GameEngineCollision* Collision_;
	};

public:
	OrbDance(); 
	~OrbDance();

	OrbDance(const OrbDance& _other) = delete; 
	OrbDance(OrbDance&& _other) = delete; 

	OrbDance& operator=(const OrbDance& _other) = delete;
	OrbDance& operator=(const OrbDance&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

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
	const float SPEED = 200.f;

private:
	std::vector<NonParryOrb> nonParryOrbs_;

	GameEngineFSM state_;

	GameEngineImageRenderer* parryRenderer_;
	GameEngineImageRenderer* summonEffectRenderer_;
	GameEngineImageRenderer* summonSparkRenderer_;

	GameEngineTransformComponent* spinTransform_;

	int sparkEffectCount_;

	float timeCounter_;
};

