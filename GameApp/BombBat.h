#pragma once

#include <GameEngineBase\GameEngineFSM.h>

#include "ParryObjectBase.h"

class GameEngineImageRendrerer;
class GameEngineCollision;
class BombBat : public ParryObjectBase
{
public:
	BombBat(); 
	~BombBat();

	BombBat(const BombBat& _other) = delete; 
	BombBat(BombBat&& _other) = delete; 

	BombBat& operator=(const BombBat& _other) = delete;
	BombBat& operator=(const BombBat&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void Initialize(bool _bLeft);

private:
	void startSummon(float _deltaTime);
	void updateSummon(float _deltaTime);

	void startFly(float _deltaTime);
	void updateFly(float _deltaTime);

	void startExplosion(float _deltaTime);
	void updateExplosion(float _deltaTime);

private:
	GameEngineFSM state_;
	GameEngineImageRendrerer* renderer_;

};

