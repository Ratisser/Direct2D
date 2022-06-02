#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class Dust : public GameEngineActor
{
public:
	Dust(); 
	~Dust();

	Dust(const Dust& _other) = delete; 
	Dust(Dust&& _other) = delete; 

	Dust& operator=(const Dust& _other) = delete;
	Dust& operator=(const Dust&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
	GameEngineTransformComponent* childTransform_;
};

