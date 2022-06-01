#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class Flower : public MonsterBase
{
public:
	Flower(); 
	~Flower();
	Flower(const Flower& _other) = delete; 
	Flower(Flower&& _other) = delete; 
	Flower& operator=(const Flower& _other) = delete;
	Flower& operator=(const Flower&& _other) = delete;

public:
	// MonsterBase을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	virtual void OnHit() override;

private:
	void initInput();
	void initTransform();
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
#pragma region Phase1
	void startIntro(float _deltaTime);
	void updateIntro(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);
	void endIdle(float _deltaTime);

	void startFaceAttackHighBegin(float _deltaTime);
	void updateFaceAttackHighBegin(float _deltaTime);
	void startFaceAttackHighIdle(float _deltaTime);
	void updateFaceAttackHighIdle(float _deltaTime);
	void startFaceAttackHighEnd(float _deltaTime);
	void updateFaceAttackHighEnd(float _deltaTime);

	void startFaceAttackLowBegin(float _deltaTime);
	void updateFaceAttackLowBegin(float _deltaTime);
	void startFaceAttackLowIdle(float _deltaTime);
	void updateFaceAttackLowIdle(float _deltaTime);
	void startFaceAttackLowEnd(float _deltaTime);
	void updateFaceAttackLowEnd(float _deltaTime);

	void startGatlingBegin(float _deltaTime);
	void updateGatlingBegin(float _deltaTime);
	void startGatlingIdle(float _deltaTime);
	void updateGatlingIdle(float _deltaTime);
	void startGatlingEnd(float _deltaTime);
	void updateGatlingEnd(float _deltaTime);

	void startBoomerangBegin(float _deltaTime);
	void updateBoomerangBegin(float _deltaTime);
	void startBoomerangAttack(float _deltaTime);
	void updateBoomerangAttack(float _deltaTime);
	void startBoomerangEnd(float _deltaTime);
	void updateBoomerangEnd(float _deltaTime);

	void startAcornBegin(float _deltaTime);
	void updateAcornBegin(float _deltaTime);
	void startAcornAttack(float _deltaTime);
	void updateAcornAttack(float _deltaTime);
	void startAcornEnd(float _deltaTime);
	void updateAcornEnd(float _deltaTime);


#pragma endregion

#pragma region Phase2
	void startPhase2Intro(float _deltaTime);
	void updatePhase2Intro(float _deltaTime);

	void startPhase2Intro2(float _deltaTime);
	void updatePhase2Intro2(float _deltaTime);

	void startPhase2Intro3(float _deltaTime);
	void updatePhase2Intro3(float _deltaTime);

	void startPhase2Idle(float _deltaTime);
	void updatePhase2Idle(float _deltaTime);

	void startSpitBegin(float _deltaTime);
	void updateSpitBegin(float _deltaTime);

	void startSpitEnd(float _deltaTime);
	void updateSpitEnd(float _deltaTime);

	void startKnockOut(float _deltaTime);
	void updateKnockOut(float _deltaTime);
#pragma endregion


private:
	enum class eAttackState { FACE_ATTACK_HIGH = 1, FACE_ATTACK_LOW, GATLING, SUMMON_OBJECT, MAX_COUNT };
	enum class eSeedColor { BLUE = 0, PURPLE, PINK, MAX_COUNT };
private:
	const int HP = 300;
	const float ACTION_DELAY = 4.0f;

	const float4 MISSILE_SPAWN_LOCATION = { 1125.f, -160.f, -0.2f };

private:
	GameEngineFSM state_;

	GameEngineTransformComponent* bodyTransform_;
	GameEngineTransformComponent* vineTransform_;
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* createObjectEffect_;
	GameEngineImageRenderer* vineRenderer_;

	GameEngineCollision* headCollision_;
	GameEngineCollision* handCollision_;
	GameEngineCollision* bodyCollision_;

	GameEngineCollision* faceAttackHighCollision_;
	GameEngineCollision* faceAttackLowCollision_;
	GameEngineCollision* vineCollision_;

	std::unique_ptr<GameEngineSoundPlayer> gatlingLoopSound_;

	int nextState_;

	float timeCounter_;
	float gatlingSeedSpawnTime_;
	float fadeOutDelay_;

	bool bPurpleSeedSpawn_;
};

