#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GatlingSeedPurple : public MonsterBase
{
public:
	GatlingSeedPurple(); 
	~GatlingSeedPurple();

	GatlingSeedPurple(const GatlingSeedPurple& _other) = delete; 
	GatlingSeedPurple(GatlingSeedPurple&& _other) = delete; 

	GatlingSeedPurple& operator=(const GatlingSeedPurple& _other) = delete;
	GatlingSeedPurple& operator=(const GatlingSeedPurple&& _other) = delete;

public:
	// MonsterBase을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	virtual void OnHit() override;

private:
	void startFall(float _deltaTime);
	void updateFall(float _deltaTime);

	void startLanding(float _deltaTime);
	void updateLanding(float _deltaTime);

	void startGrowUp(float _deltaTime);
	void updateGrowUp(float _deltaTime);


private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;

};

