#pragma once

#include <GameEngine\GameEngineLevel.h>
#include <GameEngineBase\GameEngineFSM.h>


class Player;
class Devil;
class DevilPhaseTwo;
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

	void ChangeStateEnterPhaseTwo();

private:
	void createActorPhaseTwo();

#pragma region LevelState
	void startPhaseOne(float _detaTime);
	void updatePhaseOne(float _deltaTime);

	void startEnterPhaseTwo(float _deltaTime);
	void updateEnterPhaseTwo(float _deltaTime);

	void startPhaseTwo(float _deltaTime);
	void updatePhaseTwo(float _deltaTime);

#pragma endregion

private:
	const float PHASE_TWO_BOTTOM = -4460.f;
	const float PHASE_TWO_BOTTOM_CAMERA = -4100.f;


private:
	Player* player_;
	Devil* devil_;
	DevilPhaseTwo* devilPhaseTwo_;

	GameEngineFSM levelState_;

	float4 PhaseTwoCamStartLocation_;
	float4 PhaseTwoCamEndLocation_;

	float cameraShakeTime_;
	float cameraShakeIntensity_;

	float timeCounter_;

	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

