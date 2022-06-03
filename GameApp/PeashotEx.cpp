#include "PreCompile.h"
#include "PeashotEx.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "MonsterBase.h"
#include "Map.h"

PeashotEx::PeashotEx()
	: direction_(float4::RIGHT)
	, rotation_(float4::ZERO)
	, lifeTime_(LIFE_TIME)
	, bulletRenderer_(nullptr)
	, bulletTransform_(nullptr)
	, collision_(nullptr)
	, bLeft_(false)
{

}

PeashotEx::~PeashotEx()
{

}

void PeashotEx::Start()
{
	state_.CreateState("Idle", std::bind(&PeashotEx::startIdle, this, std::placeholders::_1), std::bind(&PeashotEx::updateIdle, this, std::placeholders::_1));
	state_.CreateState("Pop", std::bind(&PeashotEx::startPop, this, std::placeholders::_1), std::bind(&PeashotEx::updatePop, this, std::placeholders::_1));
	state_.ChangeState("Idle");

	bulletTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	transform_->SetScale(0.8f, 1.0f);

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

void PeashotEx::Update(float _deltaTime)
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

void PeashotEx::InitBullet(bool _bLeft, const float4& _direction, const float4& _rotation)
{
	direction_ = _direction;
	rotation_ = _rotation;
	bLeft_ = _bLeft;
}

void PeashotEx::startIdle(float _deltaTime)
{
	bulletRenderer_->ChangeAnimation("Peashot_Loop");
	transform_->SetLocationZ(-5.0f);
}

void PeashotEx::updateIdle(float _deltaTime)
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

void PeashotEx::startPop(float _deltaTime)
{
	bulletRenderer_->ChangeAnimation("Peashot_Death");

	GameEngineSoundPlayer player("sfx_player_shoot_hit_01.wav");
	player.Play();
	player.SetVolume(0.3f);

	GameEngineSoundPlayer player2("sfx_player_weapon_peashot_death_001.wav");
	player2.Play();
	player2.SetVolume(1.0f);
}

void PeashotEx::updatePop(float _deltaTime)
{
	if (bulletRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
		return;
	}
}
