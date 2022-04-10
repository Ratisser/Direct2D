#include "PreCompile.h"
#include "Peashot.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include "MonsterBase.h"

Peashot::Peashot()
	: direction_(float4::RIGHT)
	, rotation_(float4::ZERO)
	, lifeTime_(0.5f)
	, bulletRenderer_(nullptr)
	, bulletTransform_(nullptr)
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

	bulletTransform_ = CreateTransformComponent<GameEngineTransformComponent>();

	bulletRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(bulletTransform_);
	
	transform_->SetScale(0.9f);

	bulletRenderer_->CreateAnimationFolder("Peashot_Spawn");
	bulletRenderer_->CreateAnimationFolder("Peashot_Spawn2");
	bulletRenderer_->CreateAnimationFolder("Peashot_Loop");
	bulletRenderer_->ChangeAnimation("Peashot_Spawn");

	//fireStartRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	//fireStartRenderer_->CreateAnimationFolder("Peashot_Spawn", 0.034f, false, false);
	//fireStartRenderer_->ChangeAnimation("Peashot_Spawn");
	//fireStartRenderer_->SetScale(80.f);
	//fireStartRenderer_->SetLocationY(10.f);

	//bulletRenderer_ = CreateTransformComponent<GameEngineRenderer>(transform_);
	//bulletRenderer_->SetRenderingPipeline("BoxRendering");
	//bulletRenderer_->SetScale(100.f);
	//GetTransform()->SetLocation(100, 0, 0);
}

void Peashot::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	bulletRenderer_->SetRotation(rotation_);

	if (bLeft_)
	{
		bulletRenderer_->SetFlip(true, false);
	}
	else
	{
		bulletRenderer_->SetFlip(false, false);
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
	bulletRenderer_->ChangeAnimation("Peashot_Loop");
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

	bulletTransform_->AddLocation(direction_ * BULLET_SPEED * _deltaTime);
}
