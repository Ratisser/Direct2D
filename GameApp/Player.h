#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEngineFSM.h>

class GameEngineCollision;
class GameEngineImageRenderer;
class GameEngineRenderer;
class Player : public GameEngineActor
{
public:
	Player(); 
	~Player();

	Player(const Player& _other) = delete; 
	Player(Player&& _other) = delete; 

	Player& operator=(const Player& _other) = delete;
	Player& operator=(const Player&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	void initRendererAndAnimation();
	void initInput();
	void initCollision();
	void initState();

private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startRun(float _deltaTime);
	void updateRun(float _deltaTime);

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collider_;

	bool bLeft_;
};

