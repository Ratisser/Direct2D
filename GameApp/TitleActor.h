#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class TitleActor : public GameEngineActor
{
public:
	TitleActor(); 
	~TitleActor();

	TitleActor(const TitleActor& _other) = delete; 
	TitleActor(TitleActor&& _other) = delete; 

	TitleActor& operator=(const TitleActor& _other) = delete;
	TitleActor& operator=(const TitleActor&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

protected:
	virtual void levelChangeEndEvent() override;

private:
	void startStart(float  _deltaTime);
	void updateStart(float  _deltaTime);

private:
	GameEngineImageRenderer* titleRenderer_;
	GameEngineSoundPlayer* bgmPlayer_;

	GameEngineFSM state_;

};

