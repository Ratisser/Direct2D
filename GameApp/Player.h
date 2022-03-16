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
	StateInfo startIdle(StateInfo _state);
	StateInfo updateIdle(StateInfo _state);

	StateInfo startRun(StateInfo _state);
	StateInfo updateRun(StateInfo _state);

private:
	GameEngineFSM<Player> state_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collider_;

	float deltaTime_;
	bool bLeft_;
};

