#pragma once

#include "ParryObjectBase.h"

class GameEngineImageRenderer;
class GameEngineCollision;
class FatDemonSkull : public ParryObjectBase
{
public:
	FatDemonSkull(); 
	~FatDemonSkull();

	FatDemonSkull(const FatDemonSkull& _other) = delete; 
	FatDemonSkull(FatDemonSkull&& _other) = delete; 

	FatDemonSkull& operator=(const FatDemonSkull& _other) = delete;
	FatDemonSkull& operator=(const FatDemonSkull&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	virtual void onParry() override;
public:
	void Init(bool _bLeft) { bLeft_ = _bLeft; }

private:
	GameEngineImageRenderer* renderer_;
	bool bLeft_;
	bool bParryObject_;

	float timeCounter_;
};

