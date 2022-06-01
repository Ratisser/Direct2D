#pragma once

#include "ParryObjectBase.h"

class GameEngineImageRenderer;
class Pollen : public ParryObjectBase
{
public:
	Pollen(); 
	~Pollen();
	Pollen(const Pollen& _other) = delete; 
	Pollen(Pollen&& _other) = delete; 
	Pollen& operator=(const Pollen& _other) = delete;
	Pollen& operator=(const Pollen&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

protected:
	virtual void onParry();

private:
	const float4 SPAWN_LOCATION = { 1000.f, -400.f, 0.9f };

private:
	GameEngineTransformComponent* childTransform_;
	GameEngineImageRenderer* renderer_;

	bool bParryObject_;
	float timeCounter_;
};

