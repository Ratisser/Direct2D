#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class ParryEffect : public GameEngineActor
{
public:
	ParryEffect();
	~ParryEffect();
	ParryEffect(const ParryEffect& _other) = delete;
	ParryEffect(ParryEffect&& _other) noexcept = delete;
	ParryEffect& operator=(const ParryEffect& _other) = delete;
	ParryEffect& operator=(const ParryEffect&& _other) = delete;

public:
	// GameEngineActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* parryEffectRenderer_;
};

