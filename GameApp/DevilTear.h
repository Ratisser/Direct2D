#pragma once

#include "ParryObjectBase.h"

class GameEngineImageRenderer;
class DevilTear : public ParryObjectBase
{
public:
	DevilTear();
	~DevilTear();

	DevilTear(const DevilTear& _other) = delete; 
	DevilTear(DevilTear&& _other) = delete; 

	DevilTear& operator=(const DevilTear& _other) = delete;
	DevilTear& operator=(const DevilTear&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	virtual void onParry() override;
};

