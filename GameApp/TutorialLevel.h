#pragma once

#include <GameEngine/GameEngineLevel.h>


class TutorialLevel : public GameEngineLevel
{
public:
	TutorialLevel(); 
	~TutorialLevel();
	TutorialLevel(const TutorialLevel& _other) = delete; 
	TutorialLevel(TutorialLevel&& _other) = delete; 
	TutorialLevel& operator=(const TutorialLevel& _other) = delete;
	TutorialLevel& operator=(const TutorialLevel&& _other) = delete;

public:
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;

private:
	GameEngineActor* player_;
	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

