#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class Boomerang : public MonsterBase
{
public:
	Boomerang(); 
	~Boomerang();
	Boomerang(const Boomerang& _other) = delete; 
	Boomerang(Boomerang&& _other) = delete; 
	Boomerang& operator=(const Boomerang& _other) = delete;
	Boomerang& operator=(const Boomerang&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	void startSpawn(float _deltaTime);
	void updateSpawn(float _deltaTime);

	void startFlyLeft(float _deltaTime);
	void updateFlyLeft(float _deltaTime);

	void startFlyRight(float _deltaTime);
	void updateFlyRight(float _deltaTime);

	void startDeath(float _deltaTime);
	void updateDeath(float _deltaTime);

private:
	const float MOVE_SPEED = 700.f;

	const float4 SPAWN_LOCATION = { 1000.f, -360.f, 1.1f };

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;

	std::unique_ptr<GameEngineSoundPlayer> sound_;
};

