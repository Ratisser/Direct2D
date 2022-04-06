#pragma once
#include "Map.h"

class GameEngineCollision;
class WorldMap : public Map
{
public:
	WorldMap(); 
	~WorldMap();

	WorldMap(const WorldMap& _other) = delete; 
	WorldMap(WorldMap&& _other) = delete; 

	WorldMap& operator=(const WorldMap& _other) = delete;
	WorldMap& operator=(const WorldMap&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:

};

