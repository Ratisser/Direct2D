#include "PreCompile.h"
#include "Pollen.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineImageRenderer.h>

Pollen::Pollen()
	: renderer_(nullptr)
	, childTransform_(nullptr)
	, bParryObject_(false)
	, timeCounter_(0.0f)
{

}

Pollen::~Pollen()
{

}

void Pollen::Start()
{
	ParryObjectBase::Start();

	transform_->SetLocation(SPAWN_LOCATION);

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

	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	collision_->SetParent(childTransform_);
	collision_->SetScale(30.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->SetPivot(eImagePivot::CENTER);
	renderer_->CreateAnimationFolder("Pollen_Pink", 0.0678f);
	renderer_->CreateAnimationFolder("Pollen_White", 0.0678f);

	if (bParryObject_)
	{
		renderer_->ChangeAnimation("Pollen_Pink");
	}
	else
	{
		renderer_->ChangeAnimation("Pollen_White");
	}
}

void Pollen::Update(float _deltaTime)
{
	ParryObjectBase::Update(_deltaTime);
	timeCounter_ += _deltaTime;


	transform_->AddLocation(-300.f * _deltaTime, 0.0f);
	childTransform_->SetLocationY(sinf(timeCounter_ * 7.0f) * 30.f);
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		if (bParryObject_)
		{
			renderer_->ChangeAnimation("Pollen_Pink");
		}
		else
		{
			renderer_->ChangeAnimation("Pollen_White");
		}
	}

	if (timeCounter_ > 5.0f)
	{
		Release();
	}
}

void Pollen::onParry()
{
	Release();
}
