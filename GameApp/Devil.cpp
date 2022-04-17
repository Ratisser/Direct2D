#include "PreCompile.h"
#include "Devil.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

#include "eCollisionGroup.h"

Devil::Devil()
	: devilRenderer_(nullptr)
	, dragonHeadRenderer_(nullptr)
	, leftArmRenderer_(nullptr)
	, rightArmRenderer_(nullptr)
	, pupil_(nullptr)
	, headCollision_(nullptr)
	, dragonHeadCollision_(nullptr)
	, headTransform_(nullptr)
	, leftArmTransform_(nullptr)
	, rightArmTransform_(nullptr)
	, leftDragonHeadTransform_(nullptr)
	, rightDragonHeadTransform_(nullptr)
	, timeCounter_(0.0f)
	, hitEffectTime_(0.0f)
{

}

Devil::~Devil()
{

}

void Devil::Start()
{
	GetTransform()->SetLocation(750.f, -690.f, 0.8f);

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
		devilRenderer_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
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

	leftDragonHeadTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	leftDragonHeadTransform_->SetLocation(LEFT_ARM_LOCATION);

	rightDragonHeadTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	rightDragonHeadTransform_->SetLocation(RIGHT_ARM_LOCATION);

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
	devilRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	devilRenderer_->CreateAnimationFolder("DevilIntroIdle");
	devilRenderer_->CreateAnimationFolder("DevilIntro", 0.034f, false);

	devilRenderer_->CreateAnimationFolder("DevilIdle");

	devilRenderer_->CreateAnimationFolder("RamTransform", 0.034f, false);
	devilRenderer_->CreateAnimationFolderReverse("RamTransformReverse", "RamTransform", 0.034f, false);
	devilRenderer_->CreateAnimationFolder("RamIdle");

	devilRenderer_->CreateAnimationFolder("DragonTransform", 0.034f, false);
	devilRenderer_->CreateAnimationFolderReverse("DragonTransformReverse", "DragonTransform", 0.034f, false);
	devilRenderer_->CreateAnimationFolder("DragonIdle");

	devilRenderer_->ChangeAnimation("DevilIdle");

	GameEngineTransformComponent* pupilTransfom = CreateTransformComponent<GameEngineTransformComponent>();
	pupil_ = CreateTransformComponent<GameEngineImageRenderer>(pupilTransfom);
	pupil_->CreateAnimationFolder("PupilIntro", 0.034f, false);
	pupil_->ChangeAnimation("PupilIntro");
	pupil_->SetPivot(eImagePivot::BOTTOM_LEFT);
	pupilTransfom->SetLocation(-122.f, 165.f, -0.1f);


	leftArmRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(leftArmTransform_);
	leftArmRenderer_->CreateAnimationFolder("RamArmsStart", 0.034f, false);
	leftArmRenderer_->CreateAnimationFolder("RamArmsEnd", 0.034f, false);
	leftArmRenderer_->ChangeAnimation("RamArmsStart");
	leftArmRenderer_->SetPivot(eImagePivot::CENTER);
	leftArmRenderer_->SetLocation(-465.f, 20.f, -0.1f);

	rightArmRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(rightArmTransform_);
	rightArmRenderer_->CreateAnimationFolder("RamArmsStart", 0.034f, false);
	rightArmRenderer_->CreateAnimationFolder("RamArmsEnd", 0.034f, false);
	rightArmRenderer_->ChangeAnimation("RamArmsStart");
	rightArmRenderer_->SetPivot(eImagePivot::CENTER);
	rightArmRenderer_->SetLocation(465.f, 20.f, -0.1f);
	rightArmRenderer_->SetFlip(true, false);

	dragonHeadRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(leftDragonHeadTransform_);
	dragonHeadRenderer_->SetPivot(eImagePivot::CENTER);
	dragonHeadRenderer_->SetLocation(-1050.f, 100.f, -2.0f);
	dragonHeadRenderer_->CreateAnimationFolder("DragonHeadAppear", 0.034f);
	dragonHeadRenderer_->CreateAnimationFolder("DragonHeadSmile", 0.034f, false);
	dragonHeadRenderer_->CreateAnimationFolder("DragonHeadDisappear", 0.034f, false);

	dragonHeadRenderer_->ChangeAnimation("DragonHeadAppear");
}

void Devil::initCollision()
{
	headCollision_ = CreateTransformComponent<GameEngineCollision>(headTransform_);
	headCollision_->SetCollisionType(eCollisionType::Rect);
	headCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	headCollision_->SetScale(125.f);

	dragonHeadCollision_ = CreateTransformComponent<GameEngineCollision>(leftDragonHeadTransform_);
	dragonHeadCollision_->SetCollisionType(eCollisionType::Rect);
	dragonHeadCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	dragonHeadCollision_->SetScale(80.f);
	dragonHeadCollision_->SetLocationX(-400.f);

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
	state_.CreateState("Intro", std::bind(&Devil::startIntro, this, std::placeholders::_1), std::bind(&Devil::updateIntro, this, std::placeholders::_1));

	state_.CreateState("RamTransform", std::bind(&Devil::startRamTransform, this, std::placeholders::_1), std::bind(&Devil::updateRamTransform, this, std::placeholders::_1));
	state_.CreateState("RamAttack", std::bind(&Devil::startRamAttack, this, std::placeholders::_1), std::bind(&Devil::updateRamAttack, this, std::placeholders::_1));
	state_.CreateState("RamEnd", std::bind(&Devil::startRamEnd, this, std::placeholders::_1), std::bind(&Devil::updateRamEnd, this, std::placeholders::_1));

	state_.CreateState("DragonTransform", std::bind(&Devil::startDragonTransform, this, std::placeholders::_1), std::bind(&Devil::updateDragonTransform, this, std::placeholders::_1));
	state_.CreateState("DragonAttack", std::bind(&Devil::startDragonAttack, this, std::placeholders::_1), std::bind(&Devil::updateDragonAttack, this, std::placeholders::_1));
	state_.CreateState("DragonEnd", std::bind(&Devil::startDragonEnd, this, std::placeholders::_1), std::bind(&Devil::updateDragonEnd, this, std::placeholders::_1));

	state_.ChangeState("Intro");
}

void Devil::startIntro(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("DevilIntroIdle");
	transform_->AddLocation(-61.f, 0.0f);
	pupil_->ChangeAnimation("PupilIntro", true);
}

void Devil::updateIntro(float _deltaTime)
{
	if (pupil_->GetCurrentAnimation()->IsEnd_)
	{
		pupil_->Off();
		devilRenderer_->ChangeAnimation("DevilIntro");

		if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
		{
			state_ << "Idle";
			transform_->AddLocation(61.f, 0.0f);
			return;
		}
	}
}

void Devil::startIdle(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("DevilIdle");
	devilRenderer_->SetLocationX(0.f);
	headCollision_->SetLocation(0.0f, 0.0f);
	timeCounter_ = 0;
}

void Devil::updateIdle(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > 2.f)
	{
		state_ << "DragonTransform";
		return;
	}
}

void Devil::startRamTransform(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("RamTransform");
	devilRenderer_->SetLocationX(-30.f);
	headCollision_->SetLocation(0.0f, 100.f);

	timeCounter_ = 0.0f;
}

void Devil::updateRamTransform(float _deltaTime)
{


	if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "RamAttack";
		return;
	}
}

void Devil::startRamAttack(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("RamIdle");
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
	devilRenderer_->ChangeAnimation("RamTransformReverse");
}

void Devil::updateRamEnd(float _deltaTime)
{
	if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		return;
	}
}

void Devil::startDragonTransform(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("DragonTransform");
}

void Devil::updateDragonTransform(float _deltaTime)
{
	if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "DragonAttack";
		return;
	}
}

void Devil::startDragonAttack(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("DragonIdle");
	dragonHeadRenderer_->ChangeAnimation("DragonHeadAppear", true);
	timeCounter_ = 0.f;
}

void Devil::updateDragonAttack(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ < 3.216f)
	{
		leftDragonHeadTransform_->SetLocation(GameEngineMath::Lerp(LEFT_ARM_LOCATION, RIGHT_ARM_LOCATION, timeCounter_, 3.216f));
	
	}
	else if (timeCounter_ >= 3.216f && timeCounter_ < 3.820f)
	{
		dragonHeadRenderer_->ChangeAnimation("DragonHeadSmile");
	}
	else if (timeCounter_ >= 3.820f)
	{
		dragonHeadRenderer_->ChangeAnimation("DragonHeadDisappear");
		leftDragonHeadTransform_->SetLocation(GameEngineMath::Lerp(RIGHT_ARM_LOCATION, LEFT_ARM_LOCATION, timeCounter_ - 3.820f, 0.5f));
	}


	if (timeCounter_ > 4.32f)
	{
		state_ << "DragonEnd";
		return;
	}

}

void Devil::startDragonEnd(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("DragonTransformReverse");
}

void Devil::updateDragonEnd(float _deltaTime)
{
	if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		return;
	}
}

void Devil::OnHit()
{
	devilRenderer_->SetColor({ 0.5f, 0.8f, 1.0f, 1.0f });
	hitEffectTime_ = HIT_EFFECT_TIME;
}

