#pragma once

#include <GameEngine/GameEngineActor.h>

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
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collider_;
	GameEngineRenderer* rc_;
	
};

