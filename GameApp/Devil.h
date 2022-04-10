#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class Devil : public MonsterBase
{
public:
	Devil(); 
	~Devil();

	Devil(const Devil& _other) = delete; 
	Devil(Devil&& _other) = delete; 

	Devil& operator=(const Devil& _other) = delete;
	Devil& operator=(const Devil&& _other) = delete;
	
public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
#pragma region DevilState
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startRamTransform(float _deltaTime);
	void updateRamTransform(float _deltaTime);

	void startRamAttack(float _deltaTime);
	void updateRamAttack(float _deltaTime);

	void startRamEnd(float _deltaTime);
	void updateRamEnd(float _deltaTime);

#pragma endregion


private:
	GameEngineImageRenderer* renderer_;

	GameEngineFSM state_;
};

