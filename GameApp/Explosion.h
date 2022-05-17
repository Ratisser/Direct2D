#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class Explosion : public GameEngineActor
{
public:
	Explosion(); 
	~Explosion();

	Explosion(const Explosion& _other) = delete; 
	Explosion(Explosion&& _other) = delete; 

	Explosion& operator=(const Explosion& _other) = delete;
	Explosion& operator=(const Explosion&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
};

