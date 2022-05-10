#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class Axe : public GameEngineActor
{
public:
	Axe();
	~Axe();
	Axe(const Axe& _other) = delete;
	Axe(Axe&& _other) noexcept = delete;
	Axe& operator=(const Axe& _other) = delete;
	Axe& operator=(const Axe&& _other) = delete;

public:
	// GameEngineActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
#pragma region State
	void startSummon(float _deltaTime);
	void updateSummon(float _deltaTime);

	void startSeekPlayer(float _deltaTime);
	void updateSeekPlayer(float _deltaTime);

	void startAttack(float _deltaTime);
	void updateAttack(float _deltaTime);

	void startEnd(float _deltaTime);
	void updateEnd(float _deltaTime);
#pragma endregion

private:
	const float SPIN_SPEED = 180.f;
	const float RADIUS_SPEED = 140.f;

private:
	GameEngineFSM state_;

	GameEngineTransformComponent* childTransform_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;

	float4 seekDirection_;

	float timeCounter_;
};

