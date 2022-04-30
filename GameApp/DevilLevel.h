#pragma once

#include <GameEngine\GameEngineLevel.h>

class Player;
class Devil;
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
	Player* GetPlayer() { return player_; }
	Devil* GetDevil() { return devil_; }

private:
	Player* player_;
	Devil* devil_;

	float cameraShakeTime_;
	float cameraShakeIntensity_;

	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

