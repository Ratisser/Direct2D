#pragma once

#include <GameEngineBase\GameEngineFSM.h>
#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class Acorn : public GameEngineActor
{
public:
	static int RefCount;

public:
	Acorn(); 
	~Acorn();
	Acorn(const Acorn& _other) = delete; 
	Acorn(Acorn&& _other) = delete; 
	Acorn& operator=(const Acorn& _other) = delete;
	Acorn& operator=(const Acorn&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	void startSpawn(float _deltaTime);
	void updateSpawn(float _deltaTime);

	void startFly(float _deltaTime);
	void updateFly(float _deltaTime);

	void startDeath(float _deltaTime);
	void updateDeath(float _deltaTime);

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* effect_;
	GameEngineTransformComponent* effectTransform_;
	GameEngineCollision* collision_;

	float4 direction_;
	float velocity_;
	float height_;

	float delay_;
};

