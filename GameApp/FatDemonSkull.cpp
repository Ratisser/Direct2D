#include "PreCompile.h"
#include "FatDemonSkull.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineImageRenderer.h>

FatDemonSkull::FatDemonSkull()
	: bLeft_(false)
	, timeCounter_(0.0f)
	, renderer_(nullptr)
	, bParryObject_(false)
{

}

FatDemonSkull::~FatDemonSkull()
{

}

void FatDemonSkull::Start()
{
	transform_->SetScale(0.8f);
	transform_->SetLocationZ(0.01f);
	ParryObjectBase::Start();
	GameEngineRandom random;
	bParryObject_ = static_cast<bool>(random.RandomInt(0, 1));

	if (bParryObject_)
	{
		collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	}
	else
	{
		collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	}


	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->SetPivot(eImagePivot::CENTER);
	renderer_->CreateAnimationFolder("SpawnFatDemonSkull", 0.034f, false);
	renderer_->CreateAnimationFolder("FatDemonSkull");
	renderer_->CreateAnimationFolder("SpawnFatDemonSkullParry", 0.034, false);
	renderer_->CreateAnimationFolder("FatDemonSkullParry");

	if (bParryObject_)
	{
		renderer_->ChangeAnimation("SpawnFatDemonSkullParry");
	}
	else
	{
		renderer_->ChangeAnimation("SpawnFatDemonSkull");
	}
}

void FatDemonSkull::Update(float _deltaTime)
{
	ParryObjectBase::Update(_deltaTime);
	timeCounter_ += _deltaTime;

	if (bLeft_)
	{
		transform_->AddLocation(300.f * _deltaTime, 0.0f);
	}
	else
	{
		transform_->AddLocation(-300.f * _deltaTime, 0.0f);
	}


	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		if (bParryObject_)
		{
			renderer_->ChangeAnimation("FatDemonSkullParry");
		}
		else
		{
			renderer_->ChangeAnimation("FatDemonSkull");
		}
	}

	if (timeCounter_ > 3.0f)
	{
		Release();
	}
}

void FatDemonSkull::onParry()
{
	Release();
}
