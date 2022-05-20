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

#pragma endregion

private:
	enum eAttackState { FACE_ATTACK_HIGH = 1, FACE_ATTACK_LOW, GATLING, MAX_COUNT };

private:
	const int HP = 300;
	const float ACTION_DELAY = 2.0f;

private:
	GameEngineFSM state_;

	GameEngineTransformComponent* bodyTransform_;
	GameEngineImageRenderer* renderer_;

	GameEngineCollision* headCollision_;
	GameEngineCollision* handCollision_;
	GameEngineCollision* bodyCollision_;

	GameEngineCollision* FaceAttackHighCollision_;
	GameEngineCollision* FaceAttackLowCollision_;

	int nextState_;
};

