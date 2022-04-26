#include "PreCompile.h"
#include "ParryObjectBase.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineRenderer.h>
#include "eCollisionGroup.h"

ParryObjectBase::ParryObjectBase()
	: collision_(nullptr)
	, bParryable_(true)
{

}

ParryObjectBase::~ParryObjectBase()
{

}

void ParryObjectBase::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetCollisionGroup(eCollisionGroup::ParryObject);
}

void ParryObjectBase::Update(float _deltaTime)
{
	if (bParryable_)
	{
		collision_->On();
	}
	else
	{
		collision_->Off();
	}
}

bool ParryObjectBase::IsParryable()
{
	return bParryable_;
}

void ParryObjectBase::SetParryable(bool _bParryable)
{
	bParryable_ = _bParryable;
}

void ParryObjectBase::Parry()
{
	onParry();
}
