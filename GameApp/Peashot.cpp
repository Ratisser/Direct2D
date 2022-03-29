#include "PreCompile.h"
#include "Peashot.h"

#include <GameEngine\GameEngineImageRenderer.h>

Peashot::Peashot()
	: direction_(float4::RIGHT)
	, lifeTime_(2.0f)
	, renderer_(nullptr)
	, collision_(nullptr)
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
}

void Peashot::InitBullet(const float4& _direction, float _lifeTime)
{
	direction_ = _direction;
	lifeTime_ = _lifeTime;
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
