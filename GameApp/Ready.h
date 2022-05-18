#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class Ready : public GameEngineActor
{
public:
	Ready();
	~Ready();

	Ready(const Ready& _other) = delete;
	Ready(Ready&& _other) = delete;

	Ready& operator=(const Ready& _other) = delete;
	Ready& operator=(const Ready&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;

	float timeCounter_;
};
