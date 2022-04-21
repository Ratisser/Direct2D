#include "PreCompile.h"
#include "Devil.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngineBase\GameEngineRandom.h>

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
	, spiderTransform_(nullptr)
	, spiderRenderer_(nullptr)
	, spiderCollision_(nullptr)
	, timeCounter_(0.0f)
	, hitEffectTime_(0.0f)
	, spiderFallCount_(0)
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

	initSpider();

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
		devilRenderer_->SetColor(float4::ONE);
		dragonHeadRenderer_->SetColor(float4::ONE);
		spiderRenderer_->SetColor(float4::ONE);
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

	devilRenderer_->CreateAnimationFolder("SpiderTransform", 0.034f, false);
	devilRenderer_->CreateAnimationFolderReverse("SpiderTransformReverse", "SpiderTransform", 0.034f, false);
	devilRenderer_->CreateAnimationFolder("SpiderIdle", 0.0678f);


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

	// 용 머리
	dragonHeadCollision_ = CreateTransformComponent<GameEngineCollision>(leftDragonHeadTransform_);
	dragonHeadCollision_->SetCollisionType(eCollisionType::Rect);
	dragonHeadCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	dragonHeadCollision_->SetScale(100.f);
	dragonHeadCollision_->SetLocation(-420.f, 50.f);

	// 양 손바닥
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

	state_.CreateState("SpiderTransform", std::bind(&Devil::startSpiderTransform, this, std::placeholders::_1), std::bind(&Devil::updateSpiderTransform, this, std::placeholders::_1));
	state_.CreateState("SpiderAttack", std::bind(&Devil::startSpiderAttack, this, std::placeholders::_1), std::bind(&Devil::updateSpiderAttack, this, std::placeholders::_1));
	state_.CreateState("SpiderEnd", std::bind(&Devil::startSpiderEnd, this, std::placeholders::_1), std::bind(&Devil::updateSpiderEnd, this, std::placeholders::_1));

	state_.ChangeState("Intro");
}

void Devil::initSpider()
{
	spiderTransform_ = CreateTransformComponent<GameEngineTransformComponent>(nullptr);
	spiderTransform_->SetLocationZ(0.5f);

	spiderRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(spiderTransform_);
	spiderRenderer_->CreateAnimationFolder("SpiderHead_FallFromSky");
	spiderRenderer_->CreateAnimationFolder("SpiderHead_FallToFloor", 0.067f, false);
	spiderRenderer_->CreateAnimationFolder("SpiderHead_FlyToSky", 0.034f, false);
	spiderRenderer_->CreateAnimationFolder("SpiderHead_FlyToSky2", 0.0678f);
	spiderRenderer_->ChangeAnimation("SpiderHead_FallFromSky");

	spiderCollision_ = CreateTransformComponent<GameEngineCollision>(spiderTransform_);
	spiderCollision_->SetCollisionType(eCollisionType::Rect);
	spiderCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	spiderCollision_->SetScale(200.f);
	spiderCollision_->SetLocationY(100.f);
	spiderCollision_->SetLocationZ(-0.01f);

	spiderState_.CreateState("SpiderFallFromSky", std::bind(&Devil::startSpiderFallFromSky, this, std::placeholders::_1), std::bind(&Devil::updateSpiderFallFromSky, this, std::placeholders::_1));
	spiderState_.CreateState("SpiderFallToFloor", std::bind(&Devil::startSpiderFallToFloor, this, std::placeholders::_1), std::bind(&Devil::updateSpiderFallToFloor, this, std::placeholders::_1));
	spiderState_.CreateState("SpiderFlyToSky", std::bind(&Devil::startSpiderFlyToSky, this, std::placeholders::_1), std::bind(&Devil::updateSpiderFlyToSky, this, std::placeholders::_1));
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
		//state_ << "DragonTransform";
		state_ << "SpiderTransform";
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

	if (timeCounter_ < 3.264f)
	{
		leftDragonHeadTransform_->SetLocation(GameEngineMath::Lerp(LEFT_ARM_LOCATION, RIGHT_ARM_LOCATION, timeCounter_, 3.264f));
		dragonHeadCollision_->SetLocationY(sinf(timeCounter_ * 9.0f) * 70.f + 100.f);

	}
	else if (timeCounter_ >= 3.264f && timeCounter_ < 3.868f)
	{
		devilRenderer_->Stop();
		dragonHeadRenderer_->ChangeAnimation("DragonHeadSmile");
	}
	else if (timeCounter_ >= 3.868f)
	{
		devilRenderer_->Resume();
		dragonHeadRenderer_->ChangeAnimation("DragonHeadDisappear");
		leftDragonHeadTransform_->SetLocation(GameEngineMath::Lerp(RIGHT_ARM_LOCATION, LEFT_ARM_LOCATION, timeCounter_ - 3.868f, 0.5f));
	}


	if (timeCounter_ > 4.8f)
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

void Devil::startSpiderTransform(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("SpiderTransform");
	devilRenderer_->SetLocationX(-40.f);
}

void Devil::updateSpiderTransform(float _deltaTime)
{
	if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "SpiderAttack";
	}
}

void Devil::startSpiderAttack(float _deltaTime)
{
	GameEngineRandom random;
	spiderFallCount_ = random.RandomInt(SPIDER_FALL_COUNT_MIN, SPIDER_FALL_COUNT_MAX);

	devilRenderer_->ChangeAnimation("SpiderIdle");
	spiderState_ << "SpiderFallFromSky";
}

void Devil::updateSpiderAttack(float _deltaTime)
{
	spiderState_.Update(_deltaTime);
}

void Devil::startSpiderEnd(float _deltaTime)
{
	devilRenderer_->ChangeAnimation("SpiderTransformReverse");

}

void Devil::updateSpiderEnd(float _deltaTime)
{
	if (devilRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
		return;
	}
}

void Devil::startSpiderFallFromSky(float _deltaTime)
{
	spiderRenderer_->ChangeAnimation("SpiderHead_FallFromSky", true);
	spiderRenderer_->SetLocationY(0.f);
	spiderRenderer_->SetPivot(eImagePivot::BOTTOM);
	spiderCollision_->SetLocationY(150.f);

	GameEngineRandom random;

	float spiderX = random.RandomFloat(250.f, 1200.f);

	spiderFallStartLocation_ = { spiderX, 0.0f, 0.1f };
	spiderFallFromSkyDest_ = { spiderX, -400.f, 0.1f };
	spiderFallToFloorDest_ = { spiderX, -700.f, 0.1f };
	spiderFlyToSkyDest_ = { spiderX, 0.0f, 0.1f };

	timeCounter_ = 0.0f;
}

void Devil::updateSpiderFallFromSky(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ > 1.0f)
	{
		spiderTransform_->SetLocation(GameEngineMath::Lerp(spiderFallStartLocation_, spiderFallFromSkyDest_, timeCounter_- 1.0f, 0.5f));
	}


	if (timeCounter_ > 1.6f)
	{
		spiderState_ << "SpiderFallToFloor";
		return;
	}
}

void Devil::startSpiderFallToFloor(float _deltaTime)
{
	spiderRenderer_->ChangeAnimation("SpiderHead_FallToFloor");
	timeCounter_ = 0.0f;
}

void Devil::updateSpiderFallToFloor(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	spiderTransform_->SetLocation(GameEngineMath::Lerp(spiderFallFromSkyDest_, spiderFallToFloorDest_, timeCounter_, 0.0678f));

	if (timeCounter_ > 0.5f)
	{
		spiderState_ << "SpiderFlyToSky";
		return;
	}
}

void Devil::startSpiderFlyToSky(float _deltaTime)
{
	spiderRenderer_->ChangeAnimation("SpiderHead_FlyToSky");
	timeCounter_ = 0.0f;
}

void Devil::updateSpiderFlyToSky(float _deltaTime)
{
	if (spiderRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		spiderRenderer_->ChangeAnimation("SpiderHead_FlyToSky2");
		spiderRenderer_->SetPivot(eImagePivot::CENTER);
		spiderRenderer_->SetLocationY(421.f);
		spiderCollision_->SetLocationY(421.f);
	}

	if (spiderRenderer_->GetCurrentAnimation()->Name_ == "SpiderHead_FlyToSky2")
	{
		timeCounter_ += _deltaTime;
		spiderTransform_->SetLocation(GameEngineMath::Lerp(spiderFallToFloorDest_, spiderFlyToSkyDest_, timeCounter_, 0.8f));
	}

	if (timeCounter_ > 1.f)
	{
		spiderFallCount_--;
		spiderState_ << "SpiderFallFromSky";

		if (spiderFallCount_ < 1)
		{
			state_ << "SpiderEnd";
		}

		return;
	}
}

void Devil::OnHit()
{
	const float4 onHitColor = { 0.6f, 0.8f, 1.0f, 1.0f };
	devilRenderer_->SetColor(onHitColor);
	dragonHeadRenderer_->SetColor(onHitColor);
	spiderRenderer_->SetColor(onHitColor);
	hitEffectTime_ = HIT_EFFECT_TIME;
}

