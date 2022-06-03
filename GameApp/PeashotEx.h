#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineImageRenderer;
class GameEngineRenderer;
class GameEngineCollision;
class PeashotEx : public GameEngineActor
{
public:
	PeashotEx();
	~PeashotEx();
	PeashotEx(const PeashotEx& _other) = delete;
	PeashotEx(PeashotEx&& _other) = delete;
	PeashotEx& operator=(const PeashotEx& _other) = delete;
	PeashotEx& operator=(const PeashotEx&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void InitBullet(bool _bLeft, const float4& _direction, const float4& _rotation);

private:
	void startSpawn(float _deltaTime);
	void updateSpawn(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startPop(float _deltaTime);
	void updatePop(float _deltaTime);

private:
	const float BULLET_SPEED = 1000.f;
	const int DAMAGE = 4;
	const float LIFE_TIME = 2.0f;

private:
	GameEngineImageRenderer* bulletRenderer_;
	GameEngineTransformComponent* bulletTransform_;
	GameEngineCollision* collision_;
	GameEngineFSM state_;
	float4 direction_;
	float4 rotation_;
	float lifeTime_;

	bool bLeft_;

	int hp_;
};

