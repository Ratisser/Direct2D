#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class FadeOut : public GameEngineActor
{
public:
	FadeOut(); 
	~FadeOut();

	FadeOut(const FadeOut& _other) = delete; 
	FadeOut(FadeOut&& _other) = delete; 

	FadeOut& operator=(const FadeOut& _other) = delete;
	FadeOut& operator=(const FadeOut&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
};

