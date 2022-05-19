#pragma once

#include <GameEngineBase\GameEngineFSM.h>
#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class FlowerPlatform : public GameEngineActor
{
public:
	FlowerPlatform();
	~FlowerPlatform();
	FlowerPlatform(const FlowerPlatform& _other) = delete;
	FlowerPlatform(FlowerPlatform&& _other) noexcept = delete;
	FlowerPlatform& operator=(const FlowerPlatform& _other) = delete;
	FlowerPlatform& operator=(const FlowerPlatform&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	// Appearance range : 0 ~ 2
	void SetPlatformAppearance(int _num);
	void SetPlatformMoveFactor(float _sinfValue);

private:
	void startWait(float _deltaTime);
	void updateWait(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startStop(float _deltaTime);
	void updateStop(float _deltaTime);

private:
	const float MAX_MOVE_HEIGHT = 15.f;

private:
	GameEngineFSM state_;

	GameEngineTransformComponent* rootTransform_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;

	GameEngineImageRenderer* shadowRenderer_;
	GameEngineImageRenderer* propellerRenderer_;

	float4 prevLocation_;
	float4 nextLocation_;

	float timeCounter_;
};

