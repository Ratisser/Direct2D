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
{

}

Player::~Player()
{

}

void Player::Start()
{
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

	rc_ = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	rc_->SetRenderingPipeline("BoxRendering");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->CreateAnimationFolder("TestAnimation", "Intro", 0.033f);
	renderer_->SetChangeAnimation("TestAnimation");

	rc_->SetParent(renderer_);

	collider_ = CreateTransformComponent<GameEngineCollision>(renderer_);
	collider_->SetCollisionType(eCollisionType::Rect);




	GetTransform()->SetLocation(-100, -100);
}

void Player::Update(float _deltaTime)
{
	//collider_->SetScale(renderer_->GetScale());
	//rc_->SetScale(renderer_->GetScale());
	//rc_->SetLocation(renderer_->GetLocation());


	if (nullptr != collider_->IsCollideOne(1))
	{
		GameEngineDebug::OutPutDebugString("Ãæµ¹\n");
	}
	//transform_->AddRotation(3 * -_deltaTime, 0.0f, 6 * -_deltaTime);
	//transform_->SetLocation(50.f, 0.0f, 0.0f);

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		transform_->AddLocation(0.0f, 200.f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		transform_->AddLocation(0.0f, -200.f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		transform_->AddLocation(-200.f * _deltaTime, 0.0f);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		transform_->AddLocation(200.f * _deltaTime, 0.0f);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Q"))
	{
		transform_->AddRotation(0.0f, 0.0f, 1.0f * _deltaTime);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("E"))
	{
		transform_->AddRotation(0.0f, 0.0f, -1.f * _deltaTime);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Z"))
	{
		transform_->AddScale(10.f * _deltaTime);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		transform_->AddScale(-10.f * _deltaTime);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		Release(3.f);
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
