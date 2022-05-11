#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEngineFSM.h>

class GameEngineCollision;
class GameEngineImageRenderer;
class GameEngineRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();

	Player(const Player& _other) = delete;
	Player(Player&& _other) = delete;

	Player& operator=(const Player& _other) = delete;
	Player& operator=(const Player&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	std::string GetNormalState();
	std::string GetParentState();

	void Damage();
	void ParryJump();

	void SetStateNormal();
	void SetStateCinematic();

	void SetCineState(const std::string& _cinematicStateName);

private:
	void initRendererAndAnimation();
	void initInput();
	void initCollision();
	void initState();

	void addGravity(float _deltaTime);

	void playHitSound();
	void playJumpSound();
	void playLandingSound();

private:
// Player state can controlled by other objects
#pragma region PlayerState
	void startNormalState(float _deltaTime);
	void updateNormalState(float _deltaTime);

	void startShootState(float _deltaTime);
	void updateShootState(float _deltaTime);

	void startDamagedState(float _deltaTime);
	void updateDamagedState(float _deltaTime);

	void startFallDamagedState(float _deltaTime);
	void updateFallDamagedState(float _deltaTime);

	void startCinematicState(float _deltaTime);
	void updateCinematicState(float _deltaTime);
#pragma endregion 

#pragma region PlayerNormalState
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startRun(float _deltaTime);
	void updateRun(float _deltaTime);

	void startJump(float _deltaTime);
	void updateJump(float _deltaTime);

	void startDownJump(float _deltaTime);
	void updateDownJump(float _deltaTime);

	void startDamaged(float _deltaTime);
	void updateDamaged(float _deltaTime);

	void startDash(float _deltaTime);
	void updateDash(float _deltaTime);

	void startDuck(float _deltaTime);
	void updateDuck(float _deltaTime);

	void startDuckIdle(float _deltaTime);
	void updateDuckIdle(float _deltaTime);

	void startStandUp(float _deltaTime);
	void updateStandUp(float _deltaTime);

	void startShoot(float _deltaTime);
	void updateShoot(float _deltaTime);

	void startLock(float _deltaTime);
	void updateLock(float _deltaTime);

	void startLockedShot(float _deltaTime);
	void updateLockedShot(float _deltaTime);

	void startShootWhileDucking(float _deltaTime);
	void updateShootWhileDucking(float _deltaTime);

	void startShootWhileRunning(float _deltaTime);
	void updateShootWhileRunning(float _deltaTime);

	void startParry(float _deltaTime);
	void updateParry(float _deltaTime);
	void endParry(float _deltaTime);
#pragma endregion

// Cinematic state can controlled by other objects
#pragma region PlayerCinematicState
	void startCinematicIdle(float _deltaTime);
	void updateCinematicIdle(float _deltaTime);

	void startScared(float _deltaTime);
	void updateScared(float _deltaTime);

	void startScaredWait(float _deltaTime);
	void updateScaredWait(float _deltaTime);

	void startDevilPhaseOneEndFalling(float _deltaTime);
	void updateDevilPhaseOneEndFalling(float _deltaTime);
#pragma endregion

private:
#pragma region Constant
	const float MOVE_SPEED = 500.f;
	const float GRAVITY_POWER = 5000.f;
	const float JUMP_POWER = 850.f;
	const float DASH_SPEED = 1000.f;
	const float SHOOT_DELAY = 0.12f;
	const float INVINCIBLE_TIME = 2.0f;
	const float BLINK_DELAY = 0.1f;

	const float BULLET_UP_OFFSET = 110.f;
	const float BULLET_DOWN_OFFSET = -20.f;
	const float BULLET_LEFT_OFFSET = -60.f;
	const float BULLET_RIGHT_OFFSET = 60.f;

	const float BULLET_DIAGONALUP_OFFSET = 90.f;
	const float BULLET_LEFTUP_OFFSET = -20.f;
	const float BULLET_RIGHTUP_OFFSET = 20.f;

	const float BULLET_STRAIGHT_OFFSET = 60.f;
	const float BULLET_DUCK_OFFSET = 20.f;


#pragma endregion

private:
	GameEngineFSM state_;
	GameEngineFSM normalState_;
	GameEngineFSM cinematicState_;
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* fireStartRenderer_;
	GameEngineCollision* collider_;

	GameEngineCollision* bottomCenterCollision_;
	GameEngineCollision* groundCheckCollision_;
	GameEngineCollision* fallCheckCollision_;
	GameEngineCollision* leftSideCollision_;
	GameEngineCollision* rightSideCollision_;
	GameEngineCollision* headCollision_;

	GameEngineTransformComponent* bulletSpawnParentLocation_;
	GameEngineTransformComponent* bulletSpawnLocation_;

	std::unique_ptr<GameEngineSoundPlayer> fireLoopSound_;

	float4 bulletDirection_;
	float4 bulletRotation_;

	bool bLeft_;

	bool bGround_;
	bool bCanJump_;
	bool bCanDash_;
	bool bShooting_;
	bool bDownJump_;
	bool bInvincible_;
	bool bBlink_;

	float jumpTime_;
	float gravitySpeed_;
	float shootDelay_;
	float invincibleTime_;
	float blinkTime_;

	float timeCounter_;
};

