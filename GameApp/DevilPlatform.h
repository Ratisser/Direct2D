#pragma once

#include <GameEngineBase\GameEngineFSM.h>
#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class DevilPlatform : public GameEngineActor
{
public:
	DevilPlatform();
	~DevilPlatform();
	DevilPlatform(const DevilPlatform& _other) = delete;
	DevilPlatform(DevilPlatform&& _other) noexcept = delete;
	DevilPlatform& operator=(const DevilPlatform& _other) = delete;
	DevilPlatform& operator=(const DevilPlatform&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	// Appearance range : 0 ~ 4
	void SetPlatformAppearance(int _num);


private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startMove(float _deltaTime);
	void updateMove(float _deltaTime);

	void startFall(float _deltaTime);
	void updateFall(float _deltaTime);

private:
	GameEngineFSM state_;

	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;

	float timeCounter_;
};

