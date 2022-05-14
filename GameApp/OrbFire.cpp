#include "PreCompile.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "DevilLevel.h"

#include "OrbFire.h"
#include "Player.h"
#include "Map.h"

OrbFire::OrbFire()
	: ParryObjectBase()
	, renderer_(nullptr)
	, summonEffectRenderer_(nullptr)
	, summonSparkRenderer_(nullptr)
	, sparkEffectCount_(0)
	, timeCounter_(0.0f)
	, direction_(float4::RIGHT)
	, seekDelay_(0.0f)
{

}

OrbFire::~OrbFire()
{

}

void OrbFire::Start()
{
	ParryObjectBase::Start();

	collision_->SetScale(70.f);
	collision_->SetLocationY(50.f);
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->Off();

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Orb_Fire");
	renderer_->CreateAnimationFolder("Orb_Fire_Parry");
	renderer_->ChangeAnimation("Orb_Fire");
	renderer_->Off();

	summonEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps");
	summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom", 0.034f, false);
	summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps_Parry");
	summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom_Parry", 0.034f, false);
	summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps");

	GameEngineTransformComponent* sparkTransform = CreateTransformComponent<GameEngineTransformComponent>();
	sparkTransform->SetScale(1.2f);
	sparkTransform->SetLocationY(20.f);
	summonSparkRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(sparkTransform);
	summonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark", 0.034f, false);
	summonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark_Parry", 0.034f, false);
	summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark");

	state_.CreateState("Summoning", std::bind(&OrbFire::startSummoning, this, std::placeholders::_1), std::bind(&OrbFire::updateSummoning, this, std::placeholders::_1));
	state_.CreateState("SummonComplete", std::bind(&OrbFire::startSummonComplete, this, std::placeholders::_1), std::bind(&OrbFire::updateSummonComplete, this, std::placeholders::_1));
	state_.CreateState("Move", std::bind(&OrbFire::startMove, this, std::placeholders::_1), std::bind(&OrbFire::updateMove, this, std::placeholders::_1));
	state_.CreateState("Death", std::bind(&OrbFire::startDeath, this, std::placeholders::_1), std::bind(&OrbFire::updateDeath, this, std::placeholders::_1));

	state_ << "Summoning";
}

void OrbFire::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void OrbFire::Initialize(const float4& _startPosition, float _seekDelay, bool _bParryable)
{
	transform_->SetLocation(_startPosition);
	transform_->AddLocation(0.0f, 0.0f, -0.1f);

	seekDelay_ = _seekDelay;

	SetParryable(_bParryable);

	if (_bParryable)
	{
		collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	}
}

void OrbFire::onParry()
{
	Release();
}

void OrbFire::startSummoning(float _deltaTime)
{
	if (bParryable_)
	{
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps_Parry", true);
		summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark_Parry", true);
	}
	else
	{
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps", true);
		summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark", true);
	}
	summonEffectRenderer_->On();
	summonSparkRenderer_->On();
	sparkEffectCount_ = 0;
}

void OrbFire::updateSummoning(float _deltaTime)
{
	if (summonSparkRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		summonSparkRenderer_->AddRotation(0.0f, 0.0f, 90.f);
		if (bParryable_)
		{
			summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark_Parry", true);
		}
		else
		{
			summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark", true);
		}
		sparkEffectCount_++;
	}

	if (sparkEffectCount_ >= MAX_SPARK_EFFECT_COUNT)
	{
		summonSparkRenderer_->Off();
		state_ << "SummonComplete";
		return;
	}
}

void OrbFire::startSummonComplete(float _deltaTime)
{
	if (bParryable_)
	{
		renderer_->ChangeAnimation("Orb_Fire_Parry", true);
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom_Parry");
	}
	else
	{
		renderer_->ChangeAnimation("Orb_Fire", true);
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom");
	}
	collision_->On();
	timeCounter_ = 0.0f;
}

void OrbFire::updateSummonComplete(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (summonEffectRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		summonEffectRenderer_->Off();
		renderer_->On();
	}

	if (timeCounter_ > 1.0f + seekDelay_)
	{
		summonEffectRenderer_->Off();
		renderer_->On();
		state_ << "Move";
	}
}

void OrbFire::startMove(float _deltaTime)
{
	timeCounter_ = 0.0f;
	DevilLevel* devilLevel = dynamic_cast<DevilLevel*>(level_);
	if (nullptr == devilLevel)
	{
		GameEngineDebug::MsgBoxError("DevilLevel dynamic cast failed");
		return;
	}

	Player* player = devilLevel->GetPlayer();
	float4 playerLocation = player->GetTransform()->GetWorldLocation();
	float4 orbLocation = transform_->GetWorldLocation();
	direction_ = playerLocation - orbLocation;
	direction_.Normalize3D();
}

void OrbFire::updateMove(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	transform_->AddLocation(direction_ * SPEED * _deltaTime);

	//if (float4::BLACK == Map::GetColor(left_))
	//{
	//	transform_->AddLocation(2.f, 0.0f);
	//	direction_.x *= -1;
	//}

	//if (float4::BLACK == Map::GetColor(right_))
	//{
	//	transform_->AddLocation(-2.f, 0.0f);
	//	direction_.x *= -1;
	//}

	//if (timeCounter_ < 3.0f && float4::BLACK == Map::GetColor(top_))
	//{
	//	transform_->AddLocation(0.0f, -2.0f);
	//	direction_.y *= -1;
	//}

	//if (float4::BLACK == Map::GetColor(bottom_))
	//{
	//	transform_->AddLocation(0.0f, 2.0f);
	//	direction_.y *= -1;
	//}

	if (timeCounter_ > 3.0f)
	{
		state_ << "Death";
		return;
	}
}

void OrbFire::startDeath(float _deltaTime)
{
}

void OrbFire::updateDeath(float _deltaTime)
{
	Release();
}
