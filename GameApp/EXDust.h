#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class EXDust : public GameEngineActor
{
public:
	EXDust(); 
	~EXDust();

	EXDust(const EXDust& _other) = delete; 
	EXDust(EXDust&& _other) = delete; 

	EXDust& operator=(const EXDust& _other) = delete;
	EXDust& operator=(const EXDust&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void Initialize(bool bLeft_, float _rotZAxis);

private:
	GameEngineImageRenderer* renderer_;
	GameEngineTransformComponent* childTransform_;
};

