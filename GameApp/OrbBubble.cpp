#include "PreCompile.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "OrbBubble.h"
#include "Map.h"

OrbBubble::OrbBubble()
	: ParryObjectBase()
	, renderer_(nullptr)
	, summonEffectRenderer_(nullptr)
	, summonSparkRenderer_(nullptr)
	, left_(nullptr)
	, top_(nullptr)
	, right_(nullptr)
	, bottom_(nullptr)
	, sparkEffectCount_(0)
	, timeCounter_(0.0f)
	, direction_(float4::RIGHT)
{

}

OrbBubble::~OrbBubble()
{

}

void OrbBubble::Start()
{
	ParryObjectBase::Start();

	collision_->SetScale(70.f);
	collision_->SetLocationY(50.f);
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
	collision_->Off();

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Orb_Bubble");
	renderer_->CreateAnimationFolder("Orb_Bubble_Parry");
	renderer_->ChangeAnimation("Orb_Bubble");
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

	left_ = CreateTransformComponent<GameEngineTransformComponent>();
	top_ = CreateTransformComponent<GameEngineTransformComponent>();
	right_ = CreateTransformComponent<GameEngineTransformComponent>();
	bottom_ = CreateTransformComponent<GameEngineTransformComponent>();

	left_->SetLocationX(-35.f);
	top_->SetLocationY(35.f);
	right_->SetLocationX(35.f);
	bottom_->SetLocationY(-35.f);

	state_.CreateState("Summoning", std::bind(&OrbBubble::startSummoning, this, std::placeholders::_1), std::bind(&OrbBubble::updateSummoning, this, std::placeholders::_1));
	state_.CreateState("SummonComplete", std::bind(&OrbBubble::startSummonComplete, this, std::placeholders::_1), std::bind(&OrbBubble::updateSummonComplete, this, std::placeholders::_1));
	state_.CreateState("Move", std::bind(&OrbBubble::startMove, this, std::placeholders::_1), std::bind(&OrbBubble::updateMove, this, std::placeholders::_1));
	state_.CreateState("Death", std::bind(&OrbBubble::startDeath, this, std::placeholders::_1), std::bind(&OrbBubble::updateDeath, this, std::placeholders::_1));

	state_ << "Summoning";
}

void OrbBubble::Update(float _deltaTime)
{
	state_.Update(_deltaTime);
}

void OrbBubble::Initialize(const float4& _startPosition, const float4& _direction, bool _bParryable)
{
	transform_->SetLocation(_startPosition);
	transform_->AddLocation(0.0f, 0.0f, -0.1f);

	direction_ = _direction;

	SetParryable(_bParryable);

	if (_bParryable)
	{
		collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	}
}

void OrbBubble::onParry()
{
	Release();
}

void OrbBubble::startSummoning(float _deltaTime)
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

void OrbBubble::updateSummoning(float _deltaTime)
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

void OrbBubble::startSummonComplete(float _deltaTime)
{
	if (bParryable_)
	{
		renderer_->ChangeAnimation("Orb_Bubble_Parry", true);
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom_Parry");
	}
	else
	{
		renderer_->ChangeAnimation("Orb_Bubble", true);
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom");
	}
	collision_->On();
	timeCounter_ = 0.0f;
}

void OrbBubble::updateSummonComplete(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (summonEffectRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		summonEffectRenderer_->Off();
		renderer_->On();
	}

	if (timeCounter_ > 2.0f)
	{
		state_ << "Move";
	}
}

void OrbBubble::startMove(float _deltaTime)
{
	timeCounter_ = 0.0f;
}

void OrbBubble::updateMove(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	transform_->AddLocation(direction_ * SPEED * _deltaTime);

	if (float4::BLACK == Map::GetColor(left_))
	{
		transform_->AddLocation(2.f, 0.0f);
		direction_.x *= -1;
	}

	if (float4::BLACK == Map::GetColor(right_))
	{
		transform_->AddLocation(-2.f, 0.0f);
		direction_.x *= -1;
	}

	if (timeCounter_ < 3.0f && float4::BLACK == Map::GetColor(top_))
	{
		transform_->AddLocation(0.0f, -2.0f);
		direction_.y *= -1;
	}

	if (float4::BLACK == Map::GetColor(bottom_))
	{
		transform_->AddLocation(0.0f, 2.0f);
		direction_.y *= -1;
	}

	if (timeCounter_ > 3.0f)
	{
		if (transform_->GetWorldLocation().y > 100.f)
		{
			state_ << "Death";
		}
	}
}

void OrbBubble::startDeath(float _deltaTime)
{
}

void OrbBubble::updateDeath(float _deltaTime)
{
	Release();
}
