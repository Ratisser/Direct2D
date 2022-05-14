#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class FatDemon : public MonsterBase
{
public:
	FatDemon(); 
	~FatDemon();
	FatDemon(const FatDemon& _other) = delete; 
	FatDemon(FatDemon&& _other) = delete; 
	FatDemon& operator=(const FatDemon& _other) = delete;
	FatDemon& operator=(const FatDemon&& _other) = delete;

public:
	// MonsterBase을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime);

	virtual void OnHit();

public:
	static bool Left;
	static bool Right;

private:
#pragma region State
	void startIntro(float _deltaTime);
	void updateIntro(float _deltaTime);

	void startTransition(float _deltaTime);
	void updateTransition(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startAttack(float _detlaTime);
	void updateAttack(float _detlaTime);

	void startDeath(float _deltaTime);
	void updateDeath(float _deltaTime);

	void startRelease(float _deltaTime);
	void updateRelease(float _deltaTime);

#pragma endregion


private:
	const int HP = 10;

	const float4 LEFT_SPAWN_LOCATION = { 175.f, -4700.f };
	const float4 RIGHT_SPAWN_LOCATION = { 1250.f, -4700.f };

	const float4 LEFT_START_LOCATION = { 175.f, -4100.f };
	const float4 RIGHT_START_LOCATION = { 1250.f, -4100.f };

private:
	GameEngineTransformComponent* childTransform_;
	GameEngineImageRenderer* handRenderer_;
	GameEngineImageRenderer* renderer_;

	GameEngineFSM state_;

	bool bLeft_;
};

