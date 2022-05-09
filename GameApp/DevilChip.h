#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class DevilChip : public GameEngineActor
{
public:
	DevilChip(); 
	~DevilChip();

	DevilChip(const DevilChip& _other) = delete; 
	DevilChip(DevilChip&& _other) = delete; 

	DevilChip& operator=(const DevilChip& _other) = delete;
	DevilChip& operator=(const DevilChip&& _other) = delete;

public:
	// GameEngineActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	static void FallStart();
	static void FallEnd();
	static bool IsFalling();

private:
	const float MOVE_SPEED = 400.f;

private:
	static bool bFalling_;

private:
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;
	bool bReverse_;
};

