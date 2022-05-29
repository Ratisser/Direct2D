#include "PreCompile.h"
#include "WorldMapPlayer.h"

#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>


#include "eCollisionGroup.h"
#include "WorldMap.h"


WorldMapPlayer::WorldMapPlayer()
	: renderer_(nullptr)
	, bUp_(false)
	, bDown_(true)
	, bLeft_(false)
	, bRight_(false)
	, collisionUp_(nullptr)
	, collisionDown_(nullptr)
	, collisionLeft_(nullptr)
	, collisionRight_(nullptr)
{

}

WorldMapPlayer::~WorldMapPlayer()
{

}

void WorldMapPlayer::Start()
{
	initInput();
	initRendererAndAnimation();
	initCollision();
	initState();
}

void WorldMapPlayer::Update(float _deltaTime)
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
}

void WorldMapPlayer::initInput()
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
	GameEngineInput::GetInstance().CreateKey("LShift", VK_LSHIFT);

	GameEngineInput::GetInstance().CreateKey("P", 'P');
}

void WorldMapPlayer::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("UpIdle", 0.067f);
	renderer_->CreateAnimationFolder("DownIdle", 0.067f);
	renderer_->CreateAnimationFolder("DiagonalUpIdle", 0.067f);
	renderer_->CreateAnimationFolder("DiagonalDownIdle", 0.067f);
	renderer_->CreateAnimationFolder("StraightIdle", 0.067f);
	renderer_->CreateAnimationFolder("UpWalk");
	renderer_->CreateAnimationFolder("DownWalk");
	renderer_->CreateAnimationFolder("DiagonalUpWalk");
	renderer_->CreateAnimationFolder("DiagonalDownWalk");
	renderer_->CreateAnimationFolder("StraightWalk");

	renderer_->ChangeAnimation("DownIdle");
}

void WorldMapPlayer::initCollision()
{
	collisionUp_ = CreateTransformComponent<GameEngineCollision>(transform_);
	collisionUp_->SetCollisionType(eCollisionType::Rect);
	collisionUp_->SetLocation(0.0f, 30.0f, 0.0f);

	collisionDown_ = CreateTransformComponent<GameEngineCollision>(transform_);
	collisionDown_->SetCollisionType(eCollisionType::Rect);
	collisionDown_->SetLocation(0.0f, 10.0f, 0.0f);

	collisionLeft_ = CreateTransformComponent<GameEngineCollision>(transform_);
	collisionLeft_->SetCollisionType(eCollisionType::Rect);
	collisionLeft_->SetLocation(-10.0f, 20.0f, 0.0f);

	collisionRight_ = CreateTransformComponent<GameEngineCollision>(transform_);
	collisionRight_->SetCollisionType(eCollisionType::Rect);
	collisionRight_->SetLocation(10.0f, 20.0f, 0.0f);

//#ifdef _DEBUG
//	GameEngineRenderer* newRenderer;
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(collisionUp_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(collisionDown_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(collisionLeft_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(collisionRight_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//#endif
}

void WorldMapPlayer::initState()
{
	state_.CreateState("Idle", std::bind(&WorldMapPlayer::startIdle, this, std::placeholders::_1), std::bind(&WorldMapPlayer::updateIdle, this, std::placeholders::_1));
	state_.CreateState("Move", std::bind(&WorldMapPlayer::startMove, this, std::placeholders::_1), std::bind(&WorldMapPlayer::updateMove, this, std::placeholders::_1));

	state_.ChangeState("Idle");
}

void WorldMapPlayer::startIdle(float _deltaTime)
{
	if (bUp_)
	{
		if (bLeft_ || bRight_)
		{
			renderer_->ChangeAnimation("DiagonalUpIdle");
		}
		else
		{
			renderer_->ChangeAnimation("UpIdle");
		}
	}
	else if (bDown_)
	{
		if (bLeft_ || bRight_)
		{
			renderer_->ChangeAnimation("DiagonalDownIdle");
		}
		else
		{
			renderer_->ChangeAnimation("DownIdle");
		}
	}
	else
	{
		renderer_->ChangeAnimation("StraightIdle");
	}
}

void WorldMapPlayer::updateIdle(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		state_ << "Move";
		bUp_ = true;
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		state_ << "Move";
		bDown_ = true;
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		state_ << "Move";
		bLeft_ = true;
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		state_ << "Move";
		bRight_ = true;
		return;
	}
}

void WorldMapPlayer::startMove(float _deltaTime)
{
	if (bUp_)
	{
		if (bLeft_ || bRight_)
		{
			renderer_->ChangeAnimation("DiagonalUpWalk");
		}
		else
		{
			renderer_->ChangeAnimation("UpWalk");
		}
	}
	else if (bDown_)
	{
		if (bLeft_ || bRight_)
		{
			renderer_->ChangeAnimation("DiagonalDownWalk");
		}
		else
		{
			renderer_->ChangeAnimation("DownWalk");
		}
	}
	else
	{
		renderer_->ChangeAnimation("StraightWalk");
	}
}

void WorldMapPlayer::updateMove(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyFree("Up") &&
		GameEngineInput::GetInstance().IsKeyFree("Down") &&
		GameEngineInput::GetInstance().IsKeyFree("Left") &&
		GameEngineInput::GetInstance().IsKeyFree("Right"))
	{
		state_ << "Idle";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		bUp_ = true;
	}
	else
	{
		bUp_ = false;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		bDown_ = true;
	}
	else
	{
		bDown_ = false;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
		bRight_ = false;
	}
	else
	{
		bLeft_ = false;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bRight_ = true;
		bLeft_ = false;
	}
	else
	{
		bRight_ = false;
	}

	if (bUp_)
	{
		if (bLeft_)
		{
			renderer_->ChangeAnimation("DiagonalUpWalk");
			transform_->AddLocation(-MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED, MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED);
		}
		else if (bRight_)
		{
			renderer_->ChangeAnimation("DiagonalUpWalk");
			transform_->AddLocation(MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED, MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED);
		}
		else
		{
			renderer_->ChangeAnimation("UpWalk");
			transform_->AddLocation(0.0f, MOVE_SPEED * _deltaTime);
		}
	}
	else if (bDown_)
	{
		if (bLeft_)
		{
			renderer_->ChangeAnimation("DiagonalDownWalk");
			transform_->AddLocation(-MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED, -MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED);
		}
		else if (bRight_)
		{
			renderer_->ChangeAnimation("DiagonalDownWalk");
			transform_->AddLocation(MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED, -MOVE_SPEED * _deltaTime * MOVE_DIAGONAL_SPEED);
		}
		else
		{
			renderer_->ChangeAnimation("DownWalk");
			transform_->AddLocation(0.0f, -MOVE_SPEED * _deltaTime);
		}
	}
	else
	{
		renderer_->ChangeAnimation("StraightWalk");

		if (bLeft_)
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}
		else
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}
	}

	while (float4::BLACK == Map::GetColor(collisionUp_))
	{
		transform_->AddLocation(0.0f, -1.0f);
		float4 location = transform_->GetLocation();
		transform_->SetLocationY(static_cast<float>(location.iy()));
		transform_->UpdateTransform();
	}
	while (float4::BLACK == Map::GetColor(collisionDown_))
	{
		transform_->AddLocation(0.0f, 1.0f);
		float4 location = transform_->GetLocation();
		transform_->SetLocationY(static_cast<float>(location.iy()));
		transform_->UpdateTransform();
	}
	while (float4::BLACK == Map::GetColor(collisionLeft_))
	{
		transform_->AddLocation(1.0f, 0.0f);
		float4 location = transform_->GetLocation();
		transform_->SetLocationX(static_cast<float>(location.ix()));
		transform_->UpdateTransform();
	}
	while (float4::BLACK == Map::GetColor(collisionRight_))
	{
		transform_->AddLocation(-1.0f, 0.0f);
		float4 location = transform_->GetLocation();
		transform_->SetLocationX(static_cast<float>(location.ix()));
		transform_->UpdateTransform();
	}
}
