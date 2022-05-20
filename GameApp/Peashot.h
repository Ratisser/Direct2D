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
	void InitBullet(bool _bLeft, const float4& _direction, const float4& _rotation);

private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startPop(float _deltaTime);
	void updatePop(float _deltaTime);

private:
	float BULLET_SPEED = 2500.f;
	float LIFE_TIME = 1.0f;

private:
	GameEngineImageRenderer* bulletRenderer_;
	GameEngineTransformComponent* bulletTransform_;
	GameEngineCollision* collision_;
	GameEngineFSM state_;
	float4 direction_;
	float4 rotation_;
	float lifeTime_;

	bool bLeft_;
};

