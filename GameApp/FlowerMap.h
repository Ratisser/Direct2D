#pragma once

#include "Map.h"

class GameEngineCollision;
class FlowerMap : public Map
{
public:
	FlowerMap();
	~FlowerMap();
	FlowerMap(const FlowerMap& _other) = delete;
	FlowerMap(FlowerMap&& _other) = delete;
	FlowerMap& operator=(const FlowerMap& _other) = delete;
	FlowerMap& operator=(const FlowerMap&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:

};

