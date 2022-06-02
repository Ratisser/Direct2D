#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class DashDust : public GameEngineActor
{
public:
	DashDust(); 
	~DashDust();

	DashDust(const DashDust& _other) = delete; 
	DashDust(DashDust&& _other) = delete; 

	DashDust& operator=(const DashDust& _other) = delete;
	DashDust& operator=(const DashDust&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
	GameEngineTransformComponent* childTransform_;
};

