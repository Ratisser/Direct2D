#pragma once

#include "ParryObjectBase.h"

class GameEngineImageRenderer;
class ParryObjectTutorial : public ParryObjectBase
{
public:
	ParryObjectTutorial(); 
	~ParryObjectTutorial();
	ParryObjectTutorial(const ParryObjectTutorial& _other) = delete; 
	ParryObjectTutorial(ParryObjectTutorial&& _other) = delete; 
	ParryObjectTutorial& operator=(const ParryObjectTutorial& _other) = delete;
	ParryObjectTutorial& operator=(const ParryObjectTutorial&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* parryOn_;
	GameEngineImageRenderer* parryOff_;

	float coolTime_;
};

