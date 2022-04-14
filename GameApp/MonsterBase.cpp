#include "PreCompile.h"
#include "MonsterBase.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngine\GameEngineRenderer.h>

#include "eCollisionGroup.h"

MonsterBase::MonsterBase()
	: hp_(0)
{

}

MonsterBase::~MonsterBase()
{

}

//void MonsterBase::Start()
//{
//	{
//		GameEngineRenderer* rc = CreateTransformComponent<GameEngineRenderer>(GetTransform());
//		rc->SetRenderingPipeline("BoxRendering");
//		rc->SetScale(100.f);
//	}
//
//	collider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
//	collider_->SetCollisionType(eCollisionType::Rect);
//	collider_->SetCollisionGroup(eCollisionGroup::Monster);
//	collider_->SetScale(100.f);
//
//	GetTransform()->SetLocation(100, 0, 0);
//	
//}
//
//void MonsterBase::Update(float _deltaTime)
//{
//}

void MonsterBase::SetHP(int _hp)
{
	hp_ = _hp;
}

void MonsterBase::SubtractHP(int _damage)
{
	hp_ -= _damage;
}
