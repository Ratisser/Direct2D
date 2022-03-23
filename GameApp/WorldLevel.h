#pragma once

#include "GameEngine\GameEngineLevel.h"

class WorldLevel : public GameEngineLevel
{
public:
	WorldLevel(); 
	~WorldLevel();

	WorldLevel(const WorldLevel& _other) = delete; 
	WorldLevel(WorldLevel&& _other) = delete; 

	WorldLevel& operator=(const WorldLevel& _other) = delete;
	WorldLevel& operator=(const WorldLevel&& _other) = delete;

public:
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;
};

