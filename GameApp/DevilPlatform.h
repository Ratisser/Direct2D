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

	void SetMoveable(bool _bMoveable) { bMoveable_ = _bMoveable; }


private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startMove(float _deltaTime);
	void updateMove(float _deltaTime);

	void startRevertMove(float _deltaTime);
	void updateRevertMove(float _deltaTime);

	void startFall(float _deltaTime);
	void updateFall(float _deltaTime);

private:
	enum eAction { MOVE, STAY, REVERT, MAX_COUNT };

private:
	const float ACTION_COOLTIME = 3.0f;

private:
	GameEngineFSM state_;

	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;

	float timeCounter_;

	float4 moveStartPosition_;
	float4 moveEndPosition_;

	bool bMoveable_;
};

