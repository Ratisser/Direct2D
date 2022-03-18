#pragma once

#include <GameEngine/GameEngineLevel.h>

class PlayLevel : public GameEngineLevel
{
public:
	PlayLevel(); 
	~PlayLevel();

	PlayLevel(const PlayLevel& _other) = delete; 
	PlayLevel(PlayLevel&& _other) = delete; 

	PlayLevel& operator=(const PlayLevel& _other) = delete;
	PlayLevel& operator=(const PlayLevel&& _other) = delete;

	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;

	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;
};

