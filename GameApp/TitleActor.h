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

private:
	StateInfo startStart(StateInfo _state);
	StateInfo updateStart(StateInfo _state);

private:
	GameEngineImageRenderer* titleRenderer_;
	GameEngineSoundPlayer* bgmPlayer_;

	GameEngineFSM<TitleActor> state_;

};

