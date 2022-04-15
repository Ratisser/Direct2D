#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class Devil : public MonsterBase
{
public:
	Devil(); 
	~Devil();

	Devil(const Devil& _other) = delete; 
	Devil(Devil&& _other) = delete; 

	Devil& operator=(const Devil& _other) = delete;
	Devil& operator=(const Devil&& _other) = delete;
	
public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	// MonsterBase을(를) 통해 상속됨
	virtual void OnHit() override;
private:
	void initTransform();
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
#pragma region DevilState
	void startIntro(float _deltaTime);
	void updateIntro(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startRamTransform(float _deltaTime);
	void updateRamTransform(float _deltaTime);

	void startRamAttack(float _deltaTime);
	void updateRamAttack(float _deltaTime);

	void startRamEnd(float _deltaTime);
	void updateRamEnd(float _deltaTime);

	void startDragonTransform(float _deltaTime);
	void updateDragonTransform(float _deltaTime);

	void startDragonAttack(float _deltaTime);
	void updateDragonAttack(float _deltaTime);
	
	void startDragonEnd(float _deltaTime);
	void updateDragonEnd(float _deltaTime);

#pragma endregion

private:
	const float4 HEAD_LOCATION = float4(-40.f, 450.f);
	const float4 LEFT_ARM_LOCATION = float4(-100.f, -600.f);
	const float4 RIGHT_ARM_LOCATION = float4(1542.f, -600.f);
	const float4 CENTER_ARM_LOCATION = float4(721.f, -600.f);

	const float HIT_EFFECT_TIME = 0.034f;

private:
	GameEngineImageRenderer* devilRenderer_;
	GameEngineImageRenderer* pupil_;
	GameEngineImageRenderer* leftArmRenderer_;
	GameEngineImageRenderer* rightArmRenderer_;
	GameEngineImageRenderer* dragonHeadRenderer_;

	GameEngineTransformComponent* headTransform_;
	GameEngineTransformComponent* leftArmTransform_;
	GameEngineTransformComponent* rightArmTransform_;
	GameEngineTransformComponent* leftDragonHeadTransform_;
	GameEngineTransformComponent* rightDragonHeadTransform_;

	GameEngineCollision* headCollision_;
	GameEngineCollision* dragonHeadCollision_;

	GameEngineFSM state_;

	float timeCounter_;
	float hitEffectTime_;
};

