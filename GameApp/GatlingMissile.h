#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class GatlingMissile : public GameEngineActor
{
public:
	GatlingMissile(); 
	~GatlingMissile();

	GatlingMissile(const GatlingMissile& _other) = delete; 
	GatlingMissile(GatlingMissile&& _other) = delete; 

	GatlingMissile& operator=(const GatlingMissile& _other) = delete;
	GatlingMissile& operator=(const GatlingMissile&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	

private:
	GameEngineImageRenderer* renderer_;
	int animIndex_;
	float time_;
};

