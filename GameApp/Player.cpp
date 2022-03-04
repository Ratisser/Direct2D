#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine/GameEngineInput.h>
#include <GameEngine/GameEngineLevel.h>

Player::Player()
{

}

Player::~Player()
{

}

void Player::Start()
{
	{
		GameEngineRenderer* rc = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		rc->SetRenderingPipeline("TextureBox");
		rc->SetTexture("errorTexture.png", true);
	}
	{
		GameEngineRenderer* rc = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		rc->SetRenderingPipeline("TextureBox");
		rc->SetTexture("Char.png", true);
		rc->SetLocation(0.0f, 100.f, 0.0f);
	}

	GameEngineInput::GetInstance().CreateKey("W", 'W');
	GameEngineInput::GetInstance().CreateKey("A", 'A');
	GameEngineInput::GetInstance().CreateKey("S", 'S');
	GameEngineInput::GetInstance().CreateKey("D", 'D');
	GameEngineInput::GetInstance().CreateKey("Q", 'Q');
	GameEngineInput::GetInstance().CreateKey("E", 'E');
	GameEngineInput::GetInstance().CreateKey("Z", 'Z');
	GameEngineInput::GetInstance().CreateKey("C", 'C');

	GameEngineInput::GetInstance().CreateKey("P", 'P');
}

void Player::Update(float _deltaTime)
{
	//transform_->AddRotation(3 * -_deltaTime, 0.0f, 6 * -_deltaTime);
	//transform_->SetLocation(50.f, 0.0f, 0.0f);

	if (GameEngineInput::GetInstance().IsKeyPress("W"))
	{
		transform_->AddLocation(0.0f, 100.f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("S"))
	{
		transform_->AddLocation(0.0f, -100.f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("A"))
	{
		transform_->AddLocation(-100.f * _deltaTime, 0.0f);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("D"))
	{
		transform_->AddLocation(100.f * _deltaTime, 0.0f);
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
		transform_->AddRotation(0.0f, 1.0f * _deltaTime, 0.0f);
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		transform_->AddRotation(0.0f, -1.f * _deltaTime, 0.0f);
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
