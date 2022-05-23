#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class GatlingSmokeEffect : public GameEngineActor
{
public:
	GatlingSmokeEffect(); 
	~GatlingSmokeEffect();

	GatlingSmokeEffect(const GatlingSmokeEffect& _other) = delete; 
	GatlingSmokeEffect(GatlingSmokeEffect&& _other) = delete; 

	GatlingSmokeEffect& operator=(const GatlingSmokeEffect& _other) = delete;
	GatlingSmokeEffect& operator=(const GatlingSmokeEffect&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
};

