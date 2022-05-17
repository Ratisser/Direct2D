#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class Knockout : public GameEngineActor
{
public:
	Knockout(); 
	~Knockout();

	Knockout(const Knockout& _other) = delete; 
	Knockout(Knockout&& _other) = delete; 

	Knockout& operator=(const Knockout& _other) = delete;
	Knockout& operator=(const Knockout&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
};

