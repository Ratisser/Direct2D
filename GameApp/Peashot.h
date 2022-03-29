#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineImageRenderer;
class GameEngineRenderer;
class GameEngineCollision;
class Peashot : public GameEngineActor
{
public:
	Peashot();
	~Peashot();
	Peashot(const Peashot& _other) = delete;
	Peashot(Peashot&& _other) = delete;
	Peashot& operator=(const Peashot& _other) = delete;
	Peashot& operator=(const Peashot&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void InitBullet(const float4& _direction, float _lifeTime);

private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

private:
	float BULLET_SPEED = 2000.f;

private:
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;
	GameEngineFSM state_;
	float4 direction_;
	float lifeTime_;
};

