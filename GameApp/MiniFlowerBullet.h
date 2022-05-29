#pragma once

#include <GameEngineBase\GameEngineFSM.h>
#include "ParryObjectBase.h"


class GameEngineImageRenderer;
class MiniFlowerBullet : public ParryObjectBase
{
public:
	MiniFlowerBullet(); 
	~MiniFlowerBullet();

	MiniFlowerBullet(const MiniFlowerBullet& _other) = delete; 
	MiniFlowerBullet(MiniFlowerBullet&& _other) = delete; 

	MiniFlowerBullet& operator=(const MiniFlowerBullet& _other) = delete;
	MiniFlowerBullet& operator=(const MiniFlowerBullet&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void Initialize(const float4& _direction);

protected:
	virtual void onParry();

private:
	void startFly(float _deltaTime);
	void updateFly(float _deltaTime);

	void startPop(float _deltaTime);
	void updatePop(float _deltaTime);

private:
	GameEngineFSM state_;
	
	GameEngineImageRenderer* renderer_;

	float4 direction_;

	
};

