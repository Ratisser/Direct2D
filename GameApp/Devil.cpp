#include "PreCompile.h"
#include "Devil.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "eCollisionGroup.h"

Devil::Devil()
	: renderer_(nullptr)
	, headCollision_(nullptr)
	, headTransform_(nullptr)
	, leftArmTransform_(nullptr)
	, rightArmTransform_(nullptr)
	, leftArmRenderer_(nullptr)
	, rightArmRenderer_(nullptr)
	, timeCounter_(0.0f)
	, hitEffectTime_(0.0f)
{

}

Devil::~Devil()
{

}

void Devil::Start()
{
	initTransform();
	initRendererAndAnimation();
	initCollision();
	initState();
	SetHP(100);
}

void Devil::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (hitEffectTime_ > 0.0f)
	{
		hitEffectTime_ -= _deltaTime;
	}
	else
	{
		renderer_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Devil::initTransform()
{
	headTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	headTransform_->SetLocation(HEAD_LOCATION);

	leftArmTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	leftArmTransform_->SetLocation(LEFT_ARM_LOCATION);

	rightArmTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	rightArmTransform_->SetLocation(RIGHT_ARM_LOCATION);

	//#ifdef _DEBUG
	//	GameEngineRenderer* debugRect = CreateTransformComponent<GameEngineRenderer>(headTransform_);
	//	debugRect->SetRenderingPipeline("DebugRect");
	//	debugRect->SetLocationZ(-1.f);
	//	debugRect->SetScale(3.f);
	//
	//	debugRect = CreateTransformComponent<GameEngineRenderer>(leftArmTransform_);
	//	debugRect->SetRenderingPipeline("DebugRect");
	//	debugRect->SetLocationZ(-1.f);
	//	debugRect->SetScale(3.f);
	//
	//	debugRect = CreateTransformComponent<GameEngineRenderer>(rightArmTransform_);
	//	debugRect->SetRenderingPipeline("DebugRect");
	//	debugRect->SetLocationZ(-1.f);
	//	debugRect->SetScale(3.f);
	//#endif // _DEBUG
}

void Devil::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("DevilIdle");

	renderer_->CreateAnimationFolder("RamTransform", 0.034f, false);
	renderer_->CreateAnimationFolderReverse("RamTransformReverse", "RamTransform", 0.034f, false);
	renderer_->CreateAnimationFolder("RamIdle");

	renderer_->ChangeAnimation("DevilIdle");

	leftArmRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(leftArmTransform_);
	leftArmRenderer_->CreateAnimationFolder("RamArmsStart", 0.04f, false);
	leftArmRenderer_->CreateAnimationFolder("RamArmsEnd", 0.04f, false);
	leftArmRenderer_->ChangeAnimation("RamArmsStart");
	leftArmRenderer_->SetPivot(eImagePivot::NONE);
	leftArmRenderer_->SetLocation(-465.f, 20.f, -0.1f);

	rightArmRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(rightArmTransform_);
	rightArmRenderer_->CreateAnimationFolder("RamArmsStart", 0.04f, false);
	rightArmRenderer_->CreateAnimationFolder("RamArmsEnd", 0.04f, false);
	rightArmRenderer_->ChangeAnimation("RamArmsStart");
	rightArmRenderer_->SetPivot(eImagePivot::NONE);
	rightArmRenderer_->SetLocation(465.f, 20.f, -0.1f);
	rightArmRenderer_->SetFlip(true, false);
}

void Devil::initCollision()
{
	headCollision_ = CreateTransformComponent<GameEngineCollision>(headTransform_);
	headCollision_->SetCollisionType(eCollisionType::Rect);
	headCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	headCollision_->SetScale(125.f);

	{
		GameEngineCollision* leftArmCollision = CreateTransformComponent<GameEngineCollision>(leftArmTransform_);
		leftArmCollision->SetCollisionType(eCollisionType::Rect);
		leftArmCollision->SetScale(600.f, 70.f);
		leftArmCollision->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
		leftArmCollision->SetLocationX(-300.f);

		GameEngineCollision* rightArmCollision = CreateTransformComponent<GameEngineCollision>(rightArmTransform_);
		rightArmCollision->SetCollisionType(eCollisionType::Rect);
		rightArmCollision->SetScale(600.f, 70.f);
		rightArmCollision->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
		rightArmCollision->SetLocationX(300.f);
	}

	//GameEngineCollision* test = CreateTransformComponent<GameEngineCollision>(nullptr);
	//test->SetCollisionGroup(eCollisionGroup::Monster);
	//test->SetCollisionType(eCollisionType::Rect);
	//test->SetScale(100.f);
	//test->SetLocation(400.f, -400.f);


#ifdef _DEBUG
	GameEngineRenderer* debugRect = CreateTransformComponent<GameEngineRenderer>(headCollision_);
	debugRect->SetRenderingPipeline("DebugRect");
	debugRect->SetLocationZ(-1.0f);
#endif // _DEBUG


}

void Devil::initState()
{
	//state_.CreateState("", std::bind(&Devil::start, this, std::placeholders::_1), std::bind(&Devil::update, this, std::placeholders::_1));

	state_.CreateState("Idle", std::bind(&Devil::startIdle, this, std::placeholders::_1), std::bind(&Devil::updateIdle, this, std::placeholders::_1));

	state_.CreateState("RamTransform", std::bind(&Devil::startRamTransform, this, std::placeholders::_1), std::bind(&Devil::updateRamTransform, this, std::placeholders::_1));
	state_.CreateState("RamAttack", std::bind(&Devil::startRamAttack, this, std::placeholders::_1), std::bind(&Devil::updateRamAttack, this, std::placeholders::_1));
	state_.CreateState("RamEnd", std::bind(&Devil::startRamEnd, this, std::placeholders::_1), std::bind(&Devil::updateRamEnd, this, std::placeholders::_1));

	state_.ChangeState("Idle");
}

void Devil::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("DevilIdle");
	renderer_->SetLocationX(0.f);
	headCollision_->SetLocation(0.0f, 0.0f);
	timeCounter_ = 0;
}

void Devil::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > 2.f)
	{
		state_ << "RamTransform";
		return;
	}
}

void Devil::startRamTransform(float _deltaTime)
{
	renderer_->ChangeAnimation("RamTransform");
	renderer_->SetLocationX(-30.f);
	headCollision_->SetLocation(0.0f, 100.f);

	timeCounter_ = 0.0f;
}

void Devil::updateRamTransform(float _deltaTime)
{


	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "RamAttack";
		return;
	}
}

void Devil::startRamAttack(float _deltaTime)
{
	renderer_->ChangeAnimation("RamIdle");
	leftArmRenderer_->ChangeAnimation("RamArmsStart", true);
	rightArmRenderer_->ChangeAnimation("RamArmsStart", true);
	timeCounter_ = 0;
}

void Devil::updateRamAttack(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ < 0.6f)
	{
		leftArmTransform_->SetLocation(GameEngineMath::Lerp(LEFT_ARM_LOCATION, CENTER_ARM_LOCATION, timeCounter_, 0.3f));
		rightArmTransform_->SetLocation(GameEngineMath::Lerp(RIGHT_ARM_LOCATION, CENTER_ARM_LOCATION, timeCounter_, 0.3f));
	}
	else if (timeCounter_ >= 0.6f)
	{
		leftArmRenderer_->ChangeAnimation("RamArmsEnd");
		rightArmRenderer_->ChangeAnimation("RamArmsEnd");
		leftArmTransform_->SetLocation(GameEngineMath::Lerp(CENTER_ARM_LOCATION, LEFT_ARM_LOCATION, timeCounter_ - 0.6f, 0.2f));
		rightArmTransform_->SetLocation(GameEngineMath::Lerp(CENTER_ARM_LOCATION, RIGHT_ARM_LOCATION, timeCounter_ - 0.6f, 0.2f));
	}


	if (timeCounter_ > 0.8f)
	{
		state_ << "RamEnd";
		return;
	}
}

void Devil::startRamEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("RamTransformReverse");
}

void Devil::updateRamEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		return;
	}
}

void Devil::OnHit()
{
	renderer_->SetColor({ 0.5f, 0.8f, 1.0f, 1.0f });
	hitEffectTime_ = HIT_EFFECT_TIME;
}

