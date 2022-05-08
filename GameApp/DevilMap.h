#pragma once

#include "Map.h"

// Map
// Width : 1442px
// Height : 770px
class GameEngineImageRenderer;
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

public:
	void HallFrontLayerOn();
	void HallFrontLayerOff();
	void ChangeCollisionPhaseTwo();


private:
	GameEngineImageRenderer* hallFrontLayer_;
	GameEngineImageRenderer* chairLayer_;
};

