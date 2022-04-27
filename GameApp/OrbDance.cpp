#include "PreCompile.h"

#include <GameEngine\GameEngineCollision.h>

#include "OrbDance.h"
#include "Map.h"


OrbDance::OrbDance()
	: ParryObjectBase()
	, parryRenderer_(nullptr)
	, summonEffectRenderer_(nullptr)
	, summonSparkRenderer_(nullptr)
	, spinTransform_(nullptr)
	, sparkEffectCount_(0)
	, timeCounter_(0.0f)
{

}

OrbDance::~OrbDance()
{

}

void OrbDance::Start()
{
	ParryObjectBase::Start();

	{
		collision_->SetScale(70.f);
		//collision_->SetLocationY(50.f);
		collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
		collision_->Off();

		GameEngineTransformComponent* temp = CreateTransformComponent<GameEngineTransformComponent>();
		temp->SetLocationY(-30.f);
		parryRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(temp);
		parryRenderer_->CreateAnimationFolder("Orb_Dance");
		parryRenderer_->CreateAnimationFolder("Orb_Dance_Parry");
		parryRenderer_->ChangeAnimation("Orb_Dance");
		parryRenderer_->Off();
		//parryRenderer_->SetPivot(eImagePivot::CENTER);

		summonEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(temp);
		summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps");
		summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom", 0.034f, false);
		summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps_Parry");
		summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom_Parry", 0.034f, false);
		summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps");

		GameEngineTransformComponent* sparkTransform = CreateTransformComponent<GameEngineTransformComponent>(temp);
		sparkTransform->SetScale(1.2f);
		sparkTransform->SetLocationY(20.f);
		summonSparkRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(sparkTransform);
		summonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark", 0.034f, false);
		summonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark_Parry", 0.034f, false);
		summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark");
	}

	spinTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	float4 spawnLocation = { 250.f, 0.0f };
	spawnLocation.RotateZDegree(-30.f);

	for (size_t i = 0; i < 4; i++)
	{
		GameEngineTransformComponent* baseTransform = CreateTransformComponent<GameEngineTransformComponent>(spinTransform_);;

		spawnLocation.RotateZDegree(90.f);
		baseTransform->SetLocation(spawnLocation);

		NonParryOrb newOrb = { 0, };

		newOrb.Collision_ = CreateTransformComponent<GameEngineCollision>(baseTransform);;
		newOrb.Collision_->SetCollisionType(eCollisionType::Rect);
		newOrb.Collision_->SetScale(70.f);
		newOrb.Collision_->SetLocationY(50.f);
		newOrb.Collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
		newOrb.Collision_->Off();

		newOrb.Renderer_ = CreateTransformComponent<GameEngineImageRenderer>(baseTransform);
		newOrb.Renderer_->CreateAnimationFolder("Orb_Dance");
		newOrb.Renderer_->CreateAnimationFolder("Orb_Dance_Parry");
		newOrb.Renderer_->ChangeAnimation("Orb_Dance");
		newOrb.Renderer_->Off();

		newOrb.SummonEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(baseTransform);
		newOrb.SummonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps");
		newOrb.SummonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom", 0.034f, false);
		newOrb.SummonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps_Parry");
		newOrb.SummonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom_Parry", 0.034f, false);
		newOrb.SummonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps");
		newOrb.SummonEffectRenderer_->Off();

		GameEngineTransformComponent* sparkTransform = CreateTransformComponent<GameEngineTransformComponent>(baseTransform);
		sparkTransform->SetScale(1.2f);
		sparkTransform->SetLocationY(20.f);
		newOrb.SummonSparkRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(sparkTransform);
		newOrb.SummonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark", 0.034f, false);
		newOrb.SummonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark_Parry", 0.034f, false);
		newOrb.SummonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark");
		newOrb.SummonSparkRenderer_->Off();

		nonParryOrbs_.push_back(newOrb);
	}

	state_.CreateState("Summoning", std::bind(&OrbDance::startSummoning, this, std::placeholders::_1), std::bind(&OrbDance::updateSummoning, this, std::placeholders::_1));
	state_.CreateState("SummonComplete", std::bind(&OrbDance::startSummonComplete, this, std::placeholders::_1), std::bind(&OrbDance::updateSummonComplete, this, std::placeholders::_1));
	state_.CreateState("Move", std::bind(&OrbDance::startMove, this, std::placeholders::_1), std::bind(&OrbDance::updateMove, this, std::placeholders::_1));
	state_.CreateState("Death", std::bind(&OrbDance::startDeath, this, std::placeholders::_1), std::bind(&OrbDance::updateDeath, this, std::placeholders::_1));

	state_ << "Summoning";
}

void OrbDance::Update(float _deltaTime)
{
	transform_->SetLocationZ(-0.7f);
	state_.Update(_deltaTime);
}

void OrbDance::onParry()
{
	parryRenderer_->Off();
	collision_->Off();
}

void OrbDance::startSummoning(float _deltaTime)
{
	summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps_Parry", true);
	summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark_Parry", true);
	summonEffectRenderer_->On();
	summonSparkRenderer_->On();

	for (NonParryOrb& o : nonParryOrbs_)
	{
		o.SummonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps", true);
		o.SummonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark", true);
		o.SummonEffectRenderer_->On();
		o.SummonSparkRenderer_->On();
	}

	sparkEffectCount_ = 0;
}

void OrbDance::updateSummoning(float _deltaTime)
{
	if (summonSparkRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark_Parry", true);

		for (NonParryOrb& o : nonParryOrbs_)
		{
			o.SummonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark", true);
		}

		sparkEffectCount_++;
	}

	if (sparkEffectCount_ >= MAX_SPARK_EFFECT_COUNT)
	{
		summonSparkRenderer_->Off();
		for (NonParryOrb& o : nonParryOrbs_)
		{
			o.SummonSparkRenderer_->Off();
		}

		state_ << "SummonComplete";
		return;
	}
}

void OrbDance::startSummonComplete(float _deltaTime)
{
	parryRenderer_->ChangeAnimation("Orb_Dance_Parry", true);
	summonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom_Parry");
	collision_->On();

	for (NonParryOrb& o : nonParryOrbs_)
	{
		o.Renderer_->ChangeAnimation("Orb_Dance", true);
		o.SummonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom");
		o.Collision_->On();
	}

	timeCounter_ = 0.0f;
}

void OrbDance::updateSummonComplete(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (summonEffectRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		summonEffectRenderer_->Off();
		if (IsParryable())
		{
			parryRenderer_->On();
		}
	}

	for (NonParryOrb& o : nonParryOrbs_)
	{
		if (o.SummonEffectRenderer_->GetCurrentAnimation()->IsEnd_)
		{
			o.SummonEffectRenderer_->Off();
			o.Renderer_->On();
		}
	}

	if (timeCounter_ > 2.0f)
	{
		state_ << "Move";
	}
}

void OrbDance::startMove(float _deltaTime)
{
	timeCounter_ = 0.0f;
}

void OrbDance::updateMove(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	const float SPIN_RATE = 60.f;

	transform_->AddLocation(sinf(timeCounter_ * 2.f) * SPEED * _deltaTime, 0.0f);
	spinTransform_->AddRotation(0.0f, 0.0f, SPIN_RATE * GameEngineMath::DegreeToRadian * _deltaTime);

	for (NonParryOrb& o : nonParryOrbs_)
	{
		o.Renderer_->AddRotation(0.0f, 0.0f, -SPIN_RATE * GameEngineMath::DegreeToRadian * _deltaTime);
		o.Collision_->AddRotation(0.0f, 0.0f, -SPIN_RATE * GameEngineMath::DegreeToRadian * _deltaTime);
	}

	if (timeCounter_ > 6.0f)
	{
		transform_->AddLocation(-SPEED * _deltaTime, 0.0f);

		if (transform_->GetWorldLocation().x < -400.f)
		{
			state_ << "Death";
		}
	}
}

void OrbDance::startDeath(float _deltaTime)
{
}

void OrbDance::updateDeath(float _deltaTime)
{
	Release();
}
