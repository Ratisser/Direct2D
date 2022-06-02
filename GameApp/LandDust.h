#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class LandDust : public GameEngineActor
{
public:
	LandDust(); 
	~LandDust();

	LandDust(const LandDust& _other) = delete; 
	LandDust(LandDust&& _other) = delete; 

	LandDust& operator=(const LandDust& _other) = delete;
	LandDust& operator=(const LandDust&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
	GameEngineTransformComponent* childTransform_;
};

