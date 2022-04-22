#pragma once

#include <GameEngine\GameEngineLevel.h>

class DevilLevel : public GameEngineLevel
{
public:
	DevilLevel(); 
	~DevilLevel();
	DevilLevel(const DevilLevel& _other) = delete; 
	DevilLevel(DevilLevel&& _other) = delete; 
	DevilLevel& operator=(const DevilLevel& _other) = delete;
	DevilLevel& operator=(const DevilLevel&& _other) = delete;

public:
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;

public:
	void CameraShake(float _shakeTime, float _intensity);

private:
	GameEngineActor* player_;

	float cameraShakeTime_;
	float cameraShakeIntensity_;

	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

