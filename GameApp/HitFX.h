#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class HitFX : public GameEngineActor
{
public:
	HitFX(); 
	~HitFX();

	HitFX(const HitFX& _other) = delete; 
	HitFX(HitFX&& _other) = delete; 

	HitFX& operator=(const HitFX& _other) = delete;
	HitFX& operator=(const HitFX&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
	GameEngineTransformComponent* childTransform_;
};

