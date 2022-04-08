#pragma once

#include "Map.h"

class DevilMap : public Map
{
public:
	DevilMap(); 
	~DevilMap();
	DevilMap(const DevilMap& _other) = delete; 
	DevilMap(DevilMap&& _other) = delete; 
	DevilMap& operator=(const DevilMap& _other) = delete;
	DevilMap& operator=(const DevilMap&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

};

