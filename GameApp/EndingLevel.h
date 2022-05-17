#pragma once

#include <GameEngine\GameEngineLevel.h>

class EndingLevel : public GameEngineLevel
{
public:
	EndingLevel(); 
	~EndingLevel();

	EndingLevel(const EndingLevel& _other) = delete; 
	EndingLevel(EndingLevel&& _other) = delete; 

	EndingLevel& operator=(const EndingLevel& _other) = delete;
	EndingLevel& operator=(const EndingLevel&& _other) = delete;

public:
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;

private:
	GameEngineActor* endingActor_;
};

