#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class WorldMapPlayer : public GameEngineActor
{
public:
	WorldMapPlayer(); 
	~WorldMapPlayer();
	WorldMapPlayer(const WorldMapPlayer& _other) = delete; 
	WorldMapPlayer(WorldMapPlayer&& _other) = delete; 
	WorldMapPlayer& operator=(const WorldMapPlayer& _other) = delete;
	WorldMapPlayer& operator=(const WorldMapPlayer&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	void initInput();
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startMove(float _deltaTime);
	void updateMove(float _deltaTime);

private:
	const float MOVE_SPEED = 250.f;
	const float MOVE_DIAGONAL_SPEED = 0.75f;

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;

	GameEngineCollision* collisionUp_;
	GameEngineCollision* collisionDown_;
	GameEngineCollision* collisionLeft_;
	GameEngineCollision* collisionRight_;

	bool bUp_;
	bool bDown_;
	bool bLeft_;
	bool bRight_;
};

