#include "PreCompile.h"
#include "MiniFlowerBullet.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

MiniFlowerBullet::MiniFlowerBullet()
	: renderer_(nullptr)
	, direction_(float4::DOWN)
{

}

MiniFlowerBullet::~MiniFlowerBullet()
{

}

void MiniFlowerBullet::Start()
{
	ParryObjectBase::Start();
	collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	collision_->SetScale(30.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->SetPivot(eImagePivot::CENTER);
	renderer_->CreateAnimationFolder("MiniFlowerBullet", 0.0416f);
	renderer_->CreateAnimationFolder("MiniFlowerBulletDeath", 0.0416f, false);
	renderer_->ChangeAnimation("MiniFlowerBullet");

	state_.CreateState(MakeState(MiniFlowerBullet, Fly));
	state_.CreateState(MakeState(MiniFlowerBullet, Pop));
	state_ << "Fly";
}

void MiniFlowerBullet::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void MiniFlowerBullet::Initialize(const float4& _direction)
{
	direction_ = _direction;
}

void MiniFlowerBullet::onParry()
{
	state_ << "Pop";
}

void MiniFlowerBullet::startFly(float _deltaTime)
{
	renderer_->ChangeAnimation("MiniFlowerBullet");
}

void MiniFlowerBullet::updateFly(float _deltaTime)
{
	transform_->AddLocation(direction_ * 200.f * _deltaTime);

	if (state_.GetTime() > 5.0f)
	{
		state_ << "Pop";
	}
}

void MiniFlowerBullet::startPop(float _deltaTime)
{
	collision_->Off();
	renderer_->ChangeAnimation("MiniFlowerBulletDeath");
}

void MiniFlowerBullet::updatePop(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
