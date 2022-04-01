#include "PreCompile.h"
#include "Peashot.h"

#include <GameEngine\GameEngineImageRenderer.h>

Peashot::Peashot()
	: direction_(float4::RIGHT)
	, rotation_(float4::ZERO)
	, lifeTime_(0.5f)
	, renderer_(nullptr)
	, collision_(nullptr)
	, bLeft_(false)
{

}

Peashot::~Peashot()
{

}

void Peashot::Start()
{
	state_.CreateState("Idle", std::bind(&Peashot::startIdle, this, std::placeholders::_1), std::bind(&Peashot::updateIdle, this, std::placeholders::_1));
	state_.ChangeState("Idle");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());

	renderer_->CreateAnimationFolder("Peashot_Spawn");
	renderer_->CreateAnimationFolder("Peashot_Spawn2");
	renderer_->CreateAnimationFolder("Peashot_Loop");
	renderer_->ChangeAnimation("Peashot_Spawn");

	//renderer_ = CreateTransformComponent<GameEngineRenderer>(transform_);
	//renderer_->SetRenderingPipeline("BoxRendering");
	//renderer_->SetScale(100.f);
	//GetTransform()->SetLocation(100, 0, 0);
}

void Peashot::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	renderer_->SetRotation(rotation_);

	if (bLeft_)
	{
		renderer_->SetFlip(true, false);
	}
	else
	{
		renderer_->SetFlip(false, false);
	}
}

void Peashot::InitBullet(bool _bLeft, const float4& _direction, const float4& _rotation, float _lifeTime)
{
	direction_ = _direction;
	rotation_ = _rotation;
	lifeTime_ = _lifeTime;
	bLeft_ = _bLeft;
}

void Peashot::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Peashot_Loop");
	transform_->SetLocationZ(-5.0f);
}

void Peashot::updateIdle(float _deltaTime)
{
	lifeTime_ -= _deltaTime;
	if (lifeTime_ < 0.f)
	{
		Release();
		return;
	}

	transform_->AddLocation(direction_ * BULLET_SPEED * _deltaTime);
}
