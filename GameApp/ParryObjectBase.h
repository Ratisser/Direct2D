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

protected:
	GameEngineCollision* collision_;
};

