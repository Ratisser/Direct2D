#include "PreCompile.h"
#include "Peashot.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "MonsterBase.h"
#include "Map.h"

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
	state_.CreateState("Pop", std::bind(&Peashot::startPop, this, std::placeholders::_1), std::bind(&Peashot::updatePop, this, std::placeholders::_1));
	state_.ChangeState("Idle");

	bulletTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	transform_->SetScale(0.9f);

	bulletRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(bulletTransform_);

	bulletRenderer_->CreateAnimationFolder("Peashot_Spawn");
	bulletRenderer_->CreateAnimationFolder("Peashot_Spawn2");
	bulletRenderer_->CreateAnimationFolder("Peashot_Loop");
	bulletRenderer_->CreateAnimationFolder("Peashot_Death", 0.034f, false);
	bulletRenderer_->SetPivot(eImagePivot::CENTER);
	bulletRenderer_->ChangeAnimation("Peashot_Spawn");

	collision_ = CreateTransformComponent<GameEngineCollision>(bulletTransform_);
	collision_->SetCollisionGroup(eCollisionGroup::PlayerBullet);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(30.f);

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

void Peashot::InitBullet(bool _bLeft, const float4& _direction, const float4& _rotation)
{
	direction_ = _direction;
	rotation_ = _rotation;
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

	GameEngineCollision* monsterCollision = collision_->IsCollideOne(eCollisionGroup::Monster);
	if (nullptr != monsterCollision)
	{
		MonsterBase* monster = dynamic_cast<MonsterBase*>(monsterCollision->GetActor());
		if (nullptr != monster)
		{
			monster->SubtractHP(1);
			monster->OnHit();
		}

		state_ << "Pop";
		return;
	}

	monsterCollision = collision_->IsCollideOne(eCollisionGroup::MonsterHitBox);
	if (nullptr != monsterCollision)
	{
		MonsterBase* monster = dynamic_cast<MonsterBase*>(monsterCollision->GetActor());
		if (nullptr != monster)
		{
			monster->SubtractHP(1);
			monster->OnHit();
		}

		state_ << "Pop";
		return;
	}

	
	if (float4::BLACK == Map::GetColor(collision_))
	{
		state_ << "Pop";
		return;
	}
}

void Peashot::startPop(float _deltaTime)
{
	bulletRenderer_->ChangeAnimation("Peashot_Death");
}

void Peashot::updatePop(float _deltaTime)
{
	if (bulletRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
		return;
	}
}
