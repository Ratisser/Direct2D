#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "MonsterBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class Flower : public MonsterBase
{
public:
	Flower(); 
	~Flower();
	Flower(const Flower& _other) = delete; 
	Flower(Flower&& _other) = delete; 
	Flower& operator=(const Flower& _other) = delete;
	Flower& operator=(const Flower&& _other) = delete;

public:
	// MonsterBase을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	virtual void OnHit() override;

private:
	void initInput();
	void initTransform();
	void initRendererAndAnimation();
	void initCollision();
	void initState();

private:
#pragma region State
	void startIntro(float _deltaTime);
	void updateIntro(float _deltaTime);

	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

#pragma endregion

private:
	const int HP = 400;

private:
	GameEngineFSM state_;

	GameEngineTransformComponent* bodyTransform_;
	GameEngineImageRenderer* renderer_;

};

