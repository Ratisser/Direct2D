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
#pragma region State
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

	void startSummonObjectBegin(float _deltaTime);
	void updateSummonObjectBegin(float _deltaTime);
	void startSummonObjectAttack(float _deltaTime);
	void updateSummonObjectAttack(float _deltaTime);
	void startSummonObjectEnd(float _deltaTime);
	void updateSummonObjectEnd(float _deltaTime);

#pragma endregion

private:
	enum class eAttackState { FACE_ATTACK_HIGH = 1, FACE_ATTACK_LOW, GATLING, SUMMON_OBEJCT, MAX_COUNT };
	enum class eSeedColor { BLUE = 0, PURPLE, PINK, MAX_COUNT };
private:
	const int HP = 300;
	const float ACTION_DELAY = 3.0f;

	const float4 MISSILE_SPAWN_LOCATION = { 1125.f, -160.f, -0.2f };

private:
	GameEngineFSM state_;

	GameEngineTransformComponent* bodyTransform_;
	GameEngineImageRenderer* renderer_;

	GameEngineCollision* headCollision_;
	GameEngineCollision* handCollision_;
	GameEngineCollision* bodyCollision_;

	GameEngineCollision* FaceAttackHighCollision_;
	GameEngineCollision* FaceAttackLowCollision_;

	std::unique_ptr<GameEngineSoundPlayer> gatlingLoopSound_;

	int nextState_;

	float timeCounter_;
	float gatlingSeedSpawnTime_;
};

