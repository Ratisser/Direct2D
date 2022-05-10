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

	void startIdlePhase3(float _deltaTime);
	void updateIdlePhase3(float _deltaTime);

	void startSpiralAttack(float _deltaTime);
	void updateSpiralAttack(float _deltaTime);

	void startSpiralAttackSummonAxe(float _deltaTime);
	void updateSpiralAttackSummonAxe(float _deltaTime);

	void startSpiralAttackEnd(float _deltaTime);
	void updateSpiralAttackEnd(float _deltaTime);

	void startBombAttack(float _deltaTime);
	void updateBombAttack(float _deltaTime);

#pragma endregion

private:
	enum class eAttackStatePhase2 { AXE, BOMB, MAX_COUNT };
	enum class eAttackStatePhase3 { IMP, FAT_DEMON, MAX_COUNT};

private:
	const float ACTION_DELAY = 5.0f;

private:
	GameEngineFSM state_;

	GameEngineImageRenderer* neckRenderer_;
	GameEngineImageRenderer* headRenderer_;

	GameEngineTransformComponent* neckTransform_;
	GameEngineTransformComponent* headTransform_;

	float4 BombBatSpawnLocation_;

	float timeCounter_;

	int prevState_;

	bool bLeft_;
};
