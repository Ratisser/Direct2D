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

private:
	void initRendererAndAnimation();
	void initInput();
	void initCollision();
	void initState();

	void addGravity(float _deltaTime);

	void changeToDuckIdle();
private:
	// PlayerState
#pragma region PlayerState
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startRun(float _deltaTime);
	void updateRun(float _deltaTime);

	void startJump(float _deltaTime);
	void updateJump(float _deltaTime);

	void startDamaged(float _deltaTime);
	void updateDamaged(float _deltaTime);
	
	void startDash(float _deltaTime);
	void updateDash(float _deltaTime);

	void startDuck(float _deltaTime);
	void updateDuck(float _deltaTime);

	void startDuckIdle(float _deltaTime);
	void updateDuckIdle(float _deltaTime);

	void startShoot(float _deltaTime);
	void updateShoot(float _deltaTime);

	void startLock(float _deltaTime);
	void updateLock(float _deltaTime);

	void startLockedShoot(float _deltaTime);
	void updateLockedShoot(float _deltaTime);

	void startShootWhileDucking(float _deltaTime);
	void updateShootWhileDucking(float _deltaTime);

	void startShootWhileRunning(float _deltaTime);
	void updateShootWhileRunning(float _deltaTime);

	void startShootWhileJumping(float _deltaTime);
	void updateShootWhileJumping(float _deltaTime);

#pragma endregion

private:
	const float MOVE_SPEED = 500.f;
	const float GRAVITY_POWER = 5000.f;
	const float JUMP_POWER = 800.f;

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collider_;

	GameEngineCollision* bottomCenterCollision_;
	GameEngineCollision* groundCheckCollision_;
	GameEngineCollision* leftSideCollision_;
	GameEngineCollision* rightSideCollision_;
	GameEngineCollision* headCollision_;

	bool bLeft_;

	bool bGround_;
	bool bCanJump_;
	bool bCanJumpDash_;

	float jumpTime_;
	float gravitySpeed_;
};

