#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineCollision;
class ParryObjectBase : public GameEngineActor
{
public:
	ParryObjectBase(); 
	~ParryObjectBase();
	ParryObjectBase(const ParryObjectBase& _other) = delete; 
	ParryObjectBase(ParryObjectBase&& _other) = delete; 
	ParryObjectBase& operator=(const ParryObjectBase& _other) = delete;
	ParryObjectBase& operator=(const ParryObjectBase&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;
public:
	bool IsParryable();
	void SetParryable(bool _bParryable);

protected:
	GameEngineCollision* collision_;

	bool bParryable_;
};

