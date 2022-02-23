#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderingComponent.h>

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

}

void Player::Update(float _deltaTime)
{
	transform_->AddRotation(3 * -_deltaTime, 0.0f, 6 * -_deltaTime);
	transform_->SetLocation(50.f, 0.0f, 0.0f);
}
