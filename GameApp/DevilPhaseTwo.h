#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class DevilPhaseTwo : public MonsterBase
{
public:
	DevilPhaseTwo();
	~DevilPhaseTwo();

	DevilPhaseTwo(const DevilPhaseTwo& _other) = delete;
	DevilPhaseTwo(DevilPhaseTwo&& _other) = delete;

	DevilPhaseTwo& operator=(const DevilPhaseTwo& _other) = delete;
	DevilPhaseTwo& operator=(const DevilPhaseTwo&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	// MonsterBase을(를) 통해 상속됨
	virtual void OnHit() override;

public:
	void StartIdle() { state_ << "Idle"; }

private:
	void initInput();
	void initTransform();
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
#pragma region State
	void startWait(float _deltaTime);
	void updateWait(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startEnterPhaseThree(float _deltaTime);
	void updateEnterPhaseThree(float _deltaTime);

	void startPhaseThreeIdle(float _deltaTime);
	void updatePhaseThreeIdle(float _deltaTime);

	void startEnterPhaseFour(float _deltaTime);
	void updateEnterPhaseFour(float _deltaTime);

	void startPhaseFourIdle(float _deltaTime);
	void updatePhaseFourIdle(float _deltaTime);

	void startSpiralAttack(float _deltaTime);
	void updateSpiralAttack(float _deltaTime);

	void startSpiralAttackSummonAxe(float _deltaTime);
	void updateSpiralAttackSummonAxe(float _deltaTime);

	void startSpiralAttackEnd(float _deltaTime);
	void updateSpiralAttackEnd(float _deltaTime);

	void startBombAttack(float _deltaTime);
	void updateBombAttack(float _deltaTime);

	void startSummonImp(float _deltaTime);
	void updateSummonImp(float _deltaTime);

	void startEndSummonImp(float _deltaTime);
	void updateEndSummonImp(float _deltaTime);

	void startSummonFatDemon(float _deltaTime);
	void updateSummonFatDemon(float _deltaTime);


#pragma endregion

private:
	enum class eAttackStatePhase2 { AXE, BOMB, MAX_COUNT };
	enum class eAttackStatePhase3 { IMP, FAT_DEMON, MAX_COUNT };

private:
	const float ACTION_DELAY = 4.0f;
	const float HIT_EFFECT_TIME = 0.034f;

	const int DEVIL_HP = 250;

	float4 LEFT_BOMB_SPAWN_LOCATION = { 610.f, -4050.f, -0.2f };
	float4 RIGHT_BOMB_SPAWN_LOCATION = { 840.f, -4050.f, -0.2f };

	float4 LEFT_EYE_LOCATION = { 580.f , -4000.f };
	float4 RIGHT_EYE_LOCATION = { 880.f , -4000.f };
	float4 CENTER_EYE_LOCATION = { 736.f, -4000.f };

	float4 LEFT_EYE_LOCATION_PHASE3 = { 580.f, -4000.f };
	float4 RIGHT_EYE_LOCATION_PHASE3 = { 900.f, -4000.f };



private:
	GameEngineFSM state_;

	GameEngineImageRenderer* neckRenderer_;
	GameEngineImageRenderer* headRenderer_;

	GameEngineTransformComponent* neckTransform_;
	GameEngineTransformComponent* headTransform_;

	GameEngineCollision* leftEyeCollision_;
	GameEngineCollision* rightEyeCollision_;

	float timeCounter_;
	float hitEffectTime_;

	int prevState_;

	bool bLeft_;
	bool bBombSpawned_;
};
