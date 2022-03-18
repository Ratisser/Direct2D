#include "PreCompile.h"
#include "Monster.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngine\GameEngineRenderer.h>

Monster::Monster()
	: collider_(nullptr)
{

}

Monster::~Monster()
{

}

void Monster::Start()
{
	{
		GameEngineRenderer* rc = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		rc->SetRenderingPipeline("BoxRendering");
		rc->SetScale(100.f);
	}

	collider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collider_->SetCollisionType(eCollisionType::Rect);
	collider_->SetCollisionGroup(1);
	collider_->SetScale(100.f);

	GetTransform()->SetLocation(100, 0, 0);
	
}

void Monster::Update(float _deltaTime)
{
}
