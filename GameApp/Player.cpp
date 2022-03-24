#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine/GameEngineInput.h>
#include <GameEngine/GameEngineLevel.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Map.h"

#include "eCollisionGroup.h"

Player::Player()
	: collider_(nullptr)
	, renderer_(nullptr)
	, bLeft_(false)
	, bCanJumpDash_(false)
	, bGround_(false)
	, bottomCenterCollision_(nullptr)
	, groundCheckCollision_(nullptr)
	, headCollision_(nullptr)
	, leftSideCollision_(nullptr)
	, rightSideCollision_(nullptr)
	, bCanJump_(true)
	, gravitySpeed_(0.0f)
	, jumpTime_(0.0f)
{

}

Player::~Player()
{

}

void Player::Start()
{
	initInput();
	initRendererAndAnimation();
	initState();
	initCollision();
}

void Player::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (bLeft_)
	{
		renderer_->SetFlip(true, false);
	}
	else
	{
		renderer_->SetFlip(false, false);
	}

	//if (GameEngineInput::GetInstance().IsKeyDown("P"))
	//{
	//	static bool temp = false;
	//	if (temp)
	//	{
	//		level_->GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Perspective);
	//	}
	//	else
	//	{
	//		level_->GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
	//	}
	//	temp = !temp;
	//}
}

void Player::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->CreateAnimationFolder("Intro", "Intro");
	renderer_->CreateAnimationFolder("Idle", "Idle", 0.04f);
	renderer_->CreateAnimationFolder("Run", "Run");
	renderer_->CreateAnimationFolder("Air", "Air", 0.04f);

	renderer_->CreateAnimationFolder("Duck", "Duck");
	renderer_->SetEndCallBack("Duck", std::bind(&Player::changeToDuckIdle, this));
	renderer_->CreateAnimationFolder("DuckIdle", "DuckIdle", 0.04f);
	renderer_->ChangeAnimation("Idle");

}

void Player::initInput()
{
	GameEngineInput::GetInstance().CreateKey("Left", VK_LEFT);
	GameEngineInput::GetInstance().CreateKey("Right", VK_RIGHT);
	GameEngineInput::GetInstance().CreateKey("Up", VK_UP);
	GameEngineInput::GetInstance().CreateKey("Down", VK_DOWN);
	GameEngineInput::GetInstance().CreateKey("Q", 'Q');
	GameEngineInput::GetInstance().CreateKey("E", 'E');
	GameEngineInput::GetInstance().CreateKey("Z", 'Z');
	GameEngineInput::GetInstance().CreateKey("C", 'C');
	GameEngineInput::GetInstance().CreateKey("X", 'X');

	GameEngineInput::GetInstance().CreateKey("P", 'P');
}

void Player::initCollision()
{
	collider_ = CreateTransformComponent<GameEngineCollision>(renderer_);
	collider_->SetCollisionType(eCollisionType::Rect);

	bottomCenterCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	bottomCenterCollision_->SetCollisionType(eCollisionType::Rect);
	bottomCenterCollision_->SetLocation(0.0f, 5.0f, 0.0f);
	//bottomCenterCollision_->SetScale(2.0f);

	groundCheckCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	groundCheckCollision_->SetCollisionType(eCollisionType::Rect);
	groundCheckCollision_->SetLocation(0.0f, 4.0f, 0.0f);
	//groundCheckCollision_->SetScale(2.0f);

	leftSideCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	leftSideCollision_->SetCollisionType(eCollisionType::Rect);
	leftSideCollision_->SetLocation(-50.0f, 10.0f, 0.0f);
	//leftSideCollision_->SetScale(2.0f);

	rightSideCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	rightSideCollision_->SetCollisionType(eCollisionType::Rect);
	rightSideCollision_->SetLocation(50.0f, 10.0f, 0.0f);
	//rightSideCollision_->SetScale(2.0f);

	headCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	headCollision_->SetCollisionType(eCollisionType::Rect);
	headCollision_->SetLocation(0.0f, 145.0f, 0.0f);
	//headCollision_->SetScale(2.0f);

#ifdef _DEBUG
	GameEngineRenderer* newRenderer;
	newRenderer = CreateTransformComponent<GameEngineRenderer>(bottomCenterCollision_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-1.0f);

	newRenderer = CreateTransformComponent<GameEngineRenderer>(groundCheckCollision_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-1.0f);

	newRenderer = CreateTransformComponent<GameEngineRenderer>(leftSideCollision_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-1.0f);

	newRenderer = CreateTransformComponent<GameEngineRenderer>(rightSideCollision_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-1.0f);

	newRenderer = CreateTransformComponent<GameEngineRenderer>(headCollision_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-1.0f);

	newRenderer = CreateTransformComponent<GameEngineRenderer>(collider_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-2.0f);
	newRenderer->SetScale(1.05f);
#endif
}

void Player::initState()
{
	state_.CreateState("Idle", std::bind(&Player::startIdle, this, std::placeholders::_1), std::bind(&Player::updateIdle, this, std::placeholders::_1));
	state_.CreateState("Run", std::bind(&Player::startRun, this, std::placeholders::_1), std::bind(&Player::updateRun, this, std::placeholders::_1));
	state_.CreateState("Jump", std::bind(&Player::startJump, this, std::placeholders::_1), std::bind(&Player::updateJump, this, std::placeholders::_1));
	state_.CreateState("Damaged", std::bind(&Player::startDamaged, this, std::placeholders::_1), std::bind(&Player::updateDamaged, this, std::placeholders::_1));
	state_.CreateState("Dash", std::bind(&Player::startDash, this, std::placeholders::_1), std::bind(&Player::updateDash, this, std::placeholders::_1));
	state_.CreateState("Duck", std::bind(&Player::startDuck, this, std::placeholders::_1), std::bind(&Player::updateDuck, this, std::placeholders::_1));
	state_.CreateState("DuckIdle", std::bind(&Player::startDuckIdle, this, std::placeholders::_1), std::bind(&Player::updateDuckIdle, this, std::placeholders::_1));
	state_.CreateState("Shoot", std::bind(&Player::startShoot, this, std::placeholders::_1), std::bind(&Player::updateShoot, this, std::placeholders::_1));
	state_.CreateState("Lock", std::bind(&Player::startLock, this, std::placeholders::_1), std::bind(&Player::updateLock, this, std::placeholders::_1));
	state_.CreateState("LockedShoot", std::bind(&Player::startLockedShoot, this, std::placeholders::_1), std::bind(&Player::updateLockedShoot, this, std::placeholders::_1));
	state_.CreateState("ShootWhileDucking", std::bind(&Player::startShootWhileDucking, this, std::placeholders::_1), std::bind(&Player::updateShootWhileDucking, this, std::placeholders::_1));
	state_.CreateState("ShootWhileRunning", std::bind(&Player::startShootWhileRunning, this, std::placeholders::_1), std::bind(&Player::updateShootWhileRunning, this, std::placeholders::_1));
	state_.CreateState("ShootWhileJumping", std::bind(&Player::startShootWhileJumping, this, std::placeholders::_1), std::bind(&Player::updateShootWhileJumping, this, std::placeholders::_1));

	state_.ChangeState("Idle");
}

void Player::addGravity(float _deltaTime)
{
	if (float4::BLACK != Map::GetColor(groundCheckCollision_) && float4::BLUE != Map::GetColor(groundCheckCollision_))
	{
		bGround_ = false;
		gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
	}
	else
	{
		gravitySpeed_ = 0.f;
		bGround_ = true;
		transform_->SetLocationY(static_cast<float>(static_cast<int>(transform_->GetLocation().y)));
	}
}

void Player::changeToDuckIdle()
{
	state_ << "DuckIdle";
}

void Player::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Idle");
}

void Player::updateIdle(float _deltaTime)
{
	addGravity(_deltaTime);

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
		state_ << "Run";
		return;
	}
	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bLeft_ = false;
		state_ << "Run";
		return;
	}

	if (float4::BLACK != Map::GetColor(transform_))
	{
		bGround_ = false;
		state_ << "Jump";
		return;
	}
	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		state_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		state_ << "Duck";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		state_ << "Lock";
		return;
	}
}

void Player::startRun(float _deltaTime)
{
	renderer_->ChangeAnimation("Run");
}

void Player::updateRun(float _deltaTime)
{
	addGravity(_deltaTime);

	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		state_ << "Duck";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		state_ << "Lock";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = false;
	}
	else
	{
		state_ << "Idle";
		return;
	}

	if (float4::BLACK != Map::GetColor(transform_))
	{
		bGround_ = false;
		state_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		state_ << "Jump";
		return;
	}

	//if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	//{
	//	transform_->AddLocation(0.0f, MOVE_SPEED * _deltaTime);
	//}
	//else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	//{
	//	transform_->AddLocation(0.0f, -MOVE_SPEED * _deltaTime);
	//}
	//if (GameEngineInput::GetInstance().IsKeyFree("Left") && GameEngineInput::GetInstance().IsKeyFree("Right")&&
	//	GameEngineInput::GetInstance().IsKeyFree("Up") && GameEngineInput::GetInstance().IsKeyFree("Down"))
	//{
	//	state_ << "Idle";
	//}
}

void Player::startJump(float _deltaTime)
{
	if (bGround_)
	{
		bCanJump_ = true;
		jumpTime_ = 0.2f;
		gravitySpeed_ = JUMP_POWER;
	}
	else
	{
		bCanJump_ = false;
	}

	renderer_->ChangeAnimation("Air");
}

void Player::updateJump(float _deltaTime)
{
	bGround_ = false;

	if (bCanJump_)
	{
		jumpTime_ -= _deltaTime;
		transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
		if (jumpTime_ > 0.1f)
		{

		}
		else if (true == GameEngineInput::GetInstance().IsKeyFree("Z") || jumpTime_ < 0.0f)
		{
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
			bCanJump_ = false;
		}
	}
	else
	{
		if (float4::BLACK != Map::GetColor(groundCheckCollision_))
		{
			bGround_ = false;
			transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		}
		else
		{
			bGround_ = true;
			bCanJump_ = true;
			state_ << "Idle";
			return;
		}
	}

	// ¿òÁ÷ÀÓ
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			if (float4::BLACK != Map::GetColor(leftSideCollision_))
			{
				transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
			}
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			if (float4::BLACK != Map::GetColor(rightSideCollision_))
			{
				transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
			}
			bLeft_ = false;
		}
	}
}

void Player::startDamaged(float _deltaTime)
{
}

void Player::updateDamaged(float _deltaTime)
{
}

void Player::startDash(float _deltaTime)
{
}

void Player::updateDash(float _deltaTime)
{
}

void Player::startDuck(float _deltaTime)
{
	renderer_->ChangeAnimation("Duck");
}

void Player::updateDuck(float _deltaTime)
{
}

void Player::startDuckIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("DuckIdle");
}

void Player::updateDuckIdle(float _deltaTime)
{
	if (!GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		state_ << "Idle";
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		state_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Left"))
	{
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyDown("Right"))
	{
		bLeft_ = false;
	}
}

void Player::startShoot(float _deltaTime)
{
}

void Player::updateShoot(float _deltaTime)
{
}

void Player::startLock(float _deltaTime)
{
}

void Player::updateLock(float _deltaTime)
{
}

void Player::startLockedShoot(float _deltaTime)
{
}

void Player::updateLockedShoot(float _deltaTime)
{
}

void Player::startShootWhileDucking(float _deltaTime)
{
}

void Player::updateShootWhileDucking(float _deltaTime)
{
}

void Player::startShootWhileRunning(float _deltaTime)
{
}

void Player::updateShootWhileRunning(float _deltaTime)
{
}

void Player::startShootWhileJumping(float _deltaTime)
{
}

void Player::updateShootWhileJumping(float _deltaTime)
{
}
