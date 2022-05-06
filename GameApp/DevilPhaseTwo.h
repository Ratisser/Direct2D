#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class DevilPhaseTwo : public MonsterBase
{
public:
	DevilPhaseTwo(); 
	~DevilPhaseTwo();

	DevilPhaseTwo(const DevilPhaseTwo& _other) = delete; 
	DevilPhaseTwo(DevilPhaseTwo&& _other) = delete; 

	DevilPhaseTwo& operator=(const DevilPhaseTwo& _other) = delete;
	DevilPhaseTwo& operator=(const DevilPhaseTwo&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	// MonsterBase을(를) 통해 상속됨
	virtual void OnHit() override;

private:
	void initInput();
	void initTransform();
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
#pragma region State
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);
#pragma endregion


private:
	GameEngineFSM state_;

	GameEngineImageRenderer* neckRenderer_;
	GameEngineImageRenderer* headRenderer_;
};

