#include "PreCompile.h"
#include "OrbBubble.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

OrbBubble::OrbBubble()
	: ParryObjectBase()
	, renderer_(nullptr)
	, summonEffectRenderer_(nullptr)
	, summonSparkRenderer_(nullptr)
	, sparkEffectCount_(0)
	, timeCounter_(0.0f)
{

}

OrbBubble::~OrbBubble()
{

}

void OrbBubble::Start()
{
	ParryObjectBase::Start();

	collision_->SetScale(100.f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Orb_Bubble");
	renderer_->ChangeAnimation("Orb_Bubble");
	renderer_->Off();

	summonEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_SparkZaps");
	summonEffectRenderer_->CreateAnimationFolder("OrbsSpawn_Boom", 0.034f, false);
	summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps");

	summonSparkRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	summonSparkRenderer_->CreateAnimationFolder("OrbsSpawn_Spark", 0.034f, false);
	summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark");


	state_.CreateState("Summoning", std::bind(&OrbBubble::startSummoning, this, std::placeholders::_1), std::bind(&OrbBubble::updateSummoning, this, std::placeholders::_1));
	state_.CreateState("SummonComplete", std::bind(&OrbBubble::startSummonComplete, this, std::placeholders::_1), std::bind(&OrbBubble::updateSummonComplete, this, std::placeholders::_1));
	state_.CreateState("Move", std::bind(&OrbBubble::startMove, this, std::placeholders::_1), std::bind(&OrbBubble::updateMove, this, std::placeholders::_1));
	state_.CreateState("Death", std::bind(&OrbBubble::startDeath, this, std::placeholders::_1), std::bind(&OrbBubble::updateDeath, this, std::placeholders::_1));

	state_ << "Summoning";
}

void OrbBubble::Update(float _deltaTime)
{
	ParryObjectBase::Update(_deltaTime);

	state_.Update(_deltaTime);
}

void OrbBubble::Initialize(const float4& _startPosition, bool _bParryable)
{
	transform_->SetLocation(_startPosition);
	transform_->AddLocation(0.0f, 0.0f, -0.1f);
	SetParryable(_bParryable);
	if (_bParryable)
	{
		renderer_->SetColor({ 2.0f, 0.7f, 2.0f, 1.0f });
		summonEffectRenderer_->SetColor({ 2.0f, 0.7f, 2.0f, 1.0f });
		summonSparkRenderer_->SetColor({ 2.0f, 0.7f, 2.0f, 1.0f });
	}
}

void OrbBubble::startSummoning(float _deltaTime)
{
	summonEffectRenderer_->ChangeAnimation("OrbsSpawn_SparkZaps", true);
	summonEffectRenderer_->On();
	summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark", true);
	summonSparkRenderer_->On();
	sparkEffectCount_ = 0;
}

void OrbBubble::updateSummoning(float _deltaTime)
{
	if (summonSparkRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		summonSparkRenderer_->AddRotation(0.0f, 0.0f, 90.f * GameEngineMath::DegreeToRadian);
		summonSparkRenderer_->ChangeAnimation("OrbsSpawn_Spark", true);
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
	renderer_->ChangeAnimation("Orb_Bubble", true);
	summonEffectRenderer_->ChangeAnimation("OrbsSpawn_Boom");
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


	if (timeCounter_ > 3.0f)
	{
		state_ << "Death";
	}
}

void OrbBubble::startDeath(float _deltaTime)
{
}

void OrbBubble::updateDeath(float _deltaTime)
{
	Release();
}
