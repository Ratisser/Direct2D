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

	void initSpider();

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

	void startSpiderTransform(float _deltaTime);
	void updateSpiderTransform(float _deltaTime);

	void startSpiderAttack(float _deltaTime);
	void updateSpiderAttack(float _deltaTime);

	void startSpiderEnd(float _deltaTime);
	void updateSpiderEnd(float _deltaTime);

#pragma region SubState
	void startSpiderFalling(float _deltaTime);
	void updateSpiderFalling(float _deltaTime);

	void startSpiderFall(float _deltaTime);
	void updateSpiderFall(float _deltaTime);

	void startSpiderFly(float _deltaTime);
	void updateSpiderFly(float _deltaTime);
#pragma endregion


#pragma endregion

private:
	const float4 HEAD_LOCATION = float4(-40.f, 450.f);
	const float4 LEFT_ARM_LOCATION = float4(-100.f, -600.f);
	const float4 RIGHT_ARM_LOCATION = float4(1542.f, -600.f);
	const float4 CENTER_ARM_LOCATION = float4(721.f, -600.f);

	const float HIT_EFFECT_TIME = 0.034f;

private:
	// Devil
	GameEngineTransformComponent* headTransform_;
	GameEngineImageRenderer* devilRenderer_;
	GameEngineImageRenderer* pupil_;
	GameEngineCollision* headCollision_;
	GameEngineFSM state_;

	// Ram
	GameEngineTransformComponent* leftArmTransform_;
	GameEngineTransformComponent* rightArmTransform_;
	GameEngineImageRenderer* leftArmRenderer_;
	GameEngineImageRenderer* rightArmRenderer_;

	// Dragon
	GameEngineTransformComponent* leftDragonHeadTransform_;
	GameEngineTransformComponent* rightDragonHeadTransform_;
	GameEngineImageRenderer* dragonHeadRenderer_;
	GameEngineCollision* dragonHeadCollision_;
	
	// Spider
	GameEngineTransformComponent* spiderTransform_;
	GameEngineImageRenderer* spiderRenderer_;
	GameEngineCollision* spiderCollision_;
	GameEngineFSM spiderState_;



	float timeCounter_;
	float hitEffectTime_;
};

