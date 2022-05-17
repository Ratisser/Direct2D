#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class FadeIn : public GameEngineActor
{
public:
	FadeIn(); 
	~FadeIn();

	FadeIn(const FadeIn& _other) = delete; 
	FadeIn(FadeIn&& _other) = delete; 

	FadeIn& operator=(const FadeIn& _other) = delete;
	FadeIn& operator=(const FadeIn&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
};

