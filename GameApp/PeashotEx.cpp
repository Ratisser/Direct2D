#include "PreCompile.h"
#include "PeashotEx.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "MonsterBase.h"
#include "Map.h"
#include <GameEngineBase\GameEngineRandom.h>

PeashotEx::PeashotEx()
	: direction_(float4::RIGHT)
	, rotation_(float4::ZERO)
	, lifeTime_(LIFE_TIME)
	, bulletRenderer_(nullptr)
	, bulletTransform_(nullptr)
	, collision_(nullptr)
	, bLeft_(false)
	, hp_(3)
{

}

PeashotEx::~PeashotEx()
{

}

void PeashotEx::Start()
{
	state_.CreateState(MakeState(PeashotEx, Spawn));
	state_.CreateState("Idle", std::bind(&PeashotEx::startIdle, this, std::placeholders::_1), std::bind(&PeashotEx::updateIdle, this, std::placeholders::_1));
	state_.CreateState("Pop", std::bind(&PeashotEx::startPop, this, std::placeholders::_1), std::bind(&PeashotEx::updatePop, this, std::placeholders::_1));
	state_.ChangeState("Idle");

	bulletTransform_ = CreateTransformComponent<GameEngineTransformComponent>();

	bulletRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(bulletTransform_);
	bulletRenderer_->SetLocationZ(-2.0f);

	bulletRenderer_->CreateAnimationFolder("PeaEX_Spawn", 0.0416f, false);
	bulletRenderer_->CreateAnimationFolder("PeaEX_Loop");
	bulletRenderer_->CreateAnimationFolder("PeaEX_Death", 0.0416f, false);
	bulletRenderer_->SetPivot(eImagePivot::CENTER);
	bulletRenderer_->ChangeAnimation("PeaEX_Spawn");

	collision_ = CreateTransformComponent<GameEngineCollision>(bulletRenderer_);
	collision_->SetCollisionGroup(eCollisionGroup::PlayerBullet);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetScale(0.5f, 0.8f);
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

void PeashotEx::startSpawn(float _deltaTime)
{
	bulletRenderer_->ChangeAnimation("PeaEX_Spawn");
	transform_->SetLocationZ(-3.0f);
}

void PeashotEx::updateSpawn(float _deltaTime)
{
	if (bulletRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
	}
}

void PeashotEx::startIdle(float _deltaTime)
{
	bulletRenderer_->ChangeAnimation("PeaEX_Loop");
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

	if (state_.GetTime() > 0)
	{
		GameEngineCollision* monsterCollision = collision_->IsCollideOne(eCollisionGroup::Monster);
		if (nullptr != monsterCollision)
		{
			MonsterBase* monster = dynamic_cast<MonsterBase*>(monsterCollision->GetActor());
			if (nullptr != monster)
			{
				monster->SubtractHP(DAMAGE);
				monster->OnHit();
				hp_--;

				GameEngineRandom random;
				int soundNumber = random.RandomInt(1, 3);
				std::string soundName = "sfx_player_weapon_peashoot_ex_impact_0" + std::to_string(soundNumber) + ".wav";
				GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

				state_.SetTime(-0.1f);
			}

			if (hp_ <= 0)
			{
				state_ << "Pop";
				return;
			}
		}

	}

	if (state_.GetTime() > 0)
	{
		GameEngineCollision* monsterCollision = collision_->IsCollideOne(eCollisionGroup::MonsterHitBox);
		if (nullptr != monsterCollision)
		{
			MonsterBase* monster = dynamic_cast<MonsterBase*>(monsterCollision->GetActor());
			if (nullptr != monster)
			{
				monster->SubtractHP(DAMAGE);
				monster->OnHit();
				hp_--;

				GameEngineRandom random;
				int soundNumber = random.RandomInt(1, 3);
				std::string soundName = "sfx_player_weapon_peashoot_ex_impact_0" + std::to_string(soundNumber) + ".wav";
				GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

				state_.SetTime(-0.2f);
			}

			if (hp_ <= 0)
			{
				state_ << "Pop";
				return;
			}
		}
	}


	if (float4::BLACK == Map::GetColor(collision_))
	{
		state_ << "Pop";
		return;
	}
}

void PeashotEx::startPop(float _deltaTime)
{
	bulletRenderer_->ChangeAnimation("PeaEX_Death");
}

void PeashotEx::updatePop(float _deltaTime)
{
	if (bulletRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
		return;
	}
}
