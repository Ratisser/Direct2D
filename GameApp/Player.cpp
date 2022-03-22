#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine/GameEngineInput.h>
#include <GameEngine/GameEngineLevel.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

Player::Player()
	: collider_(nullptr)
	, renderer_(nullptr)
	, bLeft_(false)
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

	if (GameEngineInput::GetInstance().IsKeyDown("P"))
	{
		static bool temp = true;
		if (temp)
		{
			level_->GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Perspective);
		}
		else
		{
			level_->GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
		}
		temp = !temp;
	}
}

void Player::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->CreateAnimationFolder("Intro", "Intro");
	renderer_->CreateAnimationFolder("Idle", "Idle", 0.067f);
	renderer_->CreateAnimationFolder("Run", "Run");
	renderer_->ChangeAnimation("Idle");

	//{
	//	GameEngineRenderer* rc = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	//	rc->SetRenderingPipeline("TextureBox");
	//	rc->SetTexture("errorTexture.png", true);
	//}
	//{
	//	GameEngineRenderer* rc = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	//	rc->SetRenderingPipeline("TextureBox");
	//	rc->SetTexture("Char.png", true);
	//	rc->SetLocation(0.0f, 100.f, 0.0f);
	//}

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
}

void Player::initState()
{
	state_.CreateState("Idle", std::bind(&Player::startIdle, this, std::placeholders::_1), std::bind(&Player::updateIdle, this, std::placeholders::_1));
	state_.CreateState("Run", std::bind(&Player::startRun, this, std::placeholders::_1), std::bind(&Player::updateRun, this, std::placeholders::_1));
	state_.ChangeState("Idle");
}

void Player::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Idle");
}

void Player::updateIdle(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
		state_ << "Run";
	}
	
	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bLeft_ = false;
		state_ << "Run";
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Q"))
	{
		transform_->AddLocation(0.0f, 0.0f, 100 * _deltaTime);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("E"))
	{
		transform_->AddLocation(0.0f, 0.0f, -100 * _deltaTime);
	}
}

void Player::startRun(float _deltaTime)
{
	renderer_->ChangeAnimation("Run");
}

void Player::updateRun(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		transform_->AddLocation(-200.f * _deltaTime, 0.0f);
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		transform_->AddLocation(200.f * _deltaTime, 0.0f);
		bLeft_ = false;
	}
	else
	{
		state_ << "Idle";
	}
}
