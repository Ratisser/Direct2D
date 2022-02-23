#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderingComponent.h>
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
		GameEngineRenderingComponent* rp = CreateTransformComponent<GameEngineRenderingComponent>(GetTransform());
		rp->SetRenderingPipeline("BoxRendering");
		rp->GetTransform()->SetScale(50.f);
	}
	{
		GameEngineRenderingComponent* rp = CreateTransformComponent<GameEngineRenderingComponent>(GetTransform());
		rp->SetRenderingPipeline("BoxRendering2");
		rp->GetTransform()->SetScale(10.f);
		rp->GetTransform()->SetLocation(0.0f, 100.f, 0.0f);
	}

	GameEngineInput::GetInstance().CreateKey("w", 'W');
	GameEngineInput::GetInstance().CreateKey("a", 'A');
	GameEngineInput::GetInstance().CreateKey("s", 'S');
	GameEngineInput::GetInstance().CreateKey("d", 'D');
	GameEngineInput::GetInstance().CreateKey("p", 'P');
}

void Player::Update(float _deltaTime)
{
	transform_->AddRotation(3 * -_deltaTime, 0.0f, 6 * -_deltaTime);
	//transform_->SetLocation(50.f, 0.0f, 0.0f);

	if (GameEngineInput::GetInstance().IsKeyPress("w"))
	{
		transform_->AddLocation(0.0f, 100.f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("s"))
	{
		transform_->AddLocation(0.0f, -100.f * _deltaTime);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("a"))
	{
		transform_->AddLocation(-100.f * _deltaTime, 0.0f);
	}
	if (GameEngineInput::GetInstance().IsKeyPress("d"))
	{
		transform_->AddLocation(100.f * _deltaTime, 0.0f);
	}

	if (GameEngineInput::GetInstance().IsKeyDown("p"))
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
