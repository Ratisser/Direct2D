#pragma once

#include <GameEngineBase\GameEngineFSM.h>
#include <GameEngine\GameEngineActor.h>

class GameEngineSoundPlayer;
class GameEngineImageRenderer;
class EndingActor : public GameEngineActor
{
public:
	EndingActor();
	~EndingActor();

	EndingActor(const EndingActor& _other) = delete;
	EndingActor(EndingActor&& _other) = delete;

	EndingActor& operator=(const EndingActor& _other) = delete;
	EndingActor& operator=(const EndingActor&& _other) = delete;

public:
	// GameEngineActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;

	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

