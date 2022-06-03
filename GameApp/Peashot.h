#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineImageRenderer;
class GameEngineRenderer;
class GameEngineCollision;
class Player;
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
	void InitBullet(bool _bLeft, const float4& _direction, const float4& _rotation, Player* _player);

private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startPop(float _deltaTime);
	void updatePop(float _deltaTime);

private:
	const float BULLET_SPEED = 2500.f;
	const float LIFE_TIME = 1.0f;

private:
	Player* player_;
	GameEngineImageRenderer* bulletRenderer_;
	GameEngineTransformComponent* bulletTransform_;
	GameEngineCollision* collision_;
	GameEngineFSM state_;
	float4 direction_;
	float4 rotation_;
	float lifeTime_;

	bool bLeft_;
};

