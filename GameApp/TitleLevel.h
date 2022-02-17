#pragma once

#include <GameEngine/GameEngineLevel.h>

class TitleLevel : public GameEngineLevel
{
public:
	TitleLevel(); 
	~TitleLevel();

	TitleLevel(const TitleLevel& _other) = delete; 
	TitleLevel(TitleLevel&& _other) = delete; 

	TitleLevel& operator=(const TitleLevel& _other) = delete;
	TitleLevel& operator=(const TitleLevel&& _other) = delete;

public:
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;

	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;
};

