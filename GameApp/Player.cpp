#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine/GameEngineInput.h>
#include <GameEngine/GameEngineLevel.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

Player::Player()
	: state_(this)
	, collider_(nullptr)
	, renderer_(nullptr)
	, bLeft_(false)
	, deltaTime_(0.0f)
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
	deltaTime_ = _deltaTime;

	state_.Update();

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
	GameEngineInput::GetInstance().CreateKey("W", 'W');
	GameEngineInput::GetInstance().CreateKey("A", 'A');
	GameEngineInput::GetInstance().CreateKey("S", 'S');
	GameEngineInput::GetInstance().CreateKey("D", 'D');
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
	state_.CreateState("Idle", &Player::startIdle, &Player::updateIdle);
	state_.CreateState("Run", &Player::startRun, &Player::updateRun);
	state_.ChangeState("Idle");
}

StateInfo Player::startIdle(StateInfo _state)
{
	renderer_->ChangeAnimation("Idle");
	return StateInfo();
}

StateInfo Player::updateIdle(StateInfo _state)
{
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
		return "Run";
	}
	
	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bLeft_ = false;
		return "Run";
	}
	return StateInfo();
}

StateInfo Player::startRun(StateInfo _state)
{
	renderer_->ChangeAnimation("Run");
	return StateInfo();
}

StateInfo Player::updateRun(StateInfo _state)
{
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		transform_->AddLocation(-200.f * deltaTime_, 0.0f);
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		transform_->AddLocation(200.f * deltaTime_, 0.0f);
		bLeft_ = false;
	}
	else
	{
		return "Idle";
	}



	return StateInfo();
}
