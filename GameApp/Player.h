#pragma once

#include <GameEngine/GameEngineActor.h>

class Player : public GameEngineActor
{
public:
	Player(); 
	~Player();

	Player(const Player& _other) = delete; 
	Player(Player&& _other) = delete; 

	Player& operator=(const Player& _other) = delete;
	Player& operator=(const Player&& _other) = delete;

	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

};

