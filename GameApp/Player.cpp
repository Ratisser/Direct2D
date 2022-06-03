#include "PreCompile.h"
#include "Player.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCameraComponent.h>
#include <GameEngine/GameEngineInput.h>
#include <GameEngine/GameEngineLevel.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include <GameEngineBase\GameEngineSoundPlayer.h>

#include "Map.h"

#include "eCollisionGroup.h"
#include "Peashot.h"
#include "ParryObjectBase.h"
#include <GameEngineBase\GameEngineRandom.h>
#include <GameApp\ParryEffect.h>
#include <GameApp\HitFX.h>
#include <GameApp\DashDust.h>
#include <GameApp\LandDust.h>
#include <GameApp\Dust.h>
#include <GameApp\EXDust.h>
#include "PeashotEx.h"

Player::Player()
	: collider_(nullptr)
	, renderer_(nullptr)
	, fireStartRenderer_(nullptr)
	, hpRenderer_(nullptr)
	, bLeft_(false)
	, bUpOrDown_(false)
	, bUp_(false)
	, bCanDash_(false)
	, bGround_(false)
	, bottomCenterCollision_(nullptr)
	, groundCheckCollision_(nullptr)
	, fallCheckCollision_(nullptr)
	, headCollision_(nullptr)
	, leftSideCollision_(nullptr)
	, rightSideCollision_(nullptr)
	, fireLoopSound_(nullptr)
	, bCanJump_(true)
	, bDownJump_(false)
	, bShooting_(false)
	, bInvincible_(false)
	, bBlink_(false)
	, gravitySpeed_(0.0f)
	, jumpTime_(0.0f)
	, shootDelay_(0.0f)
	, invincibleTime_(0.0f)
	, blinkTime_(0.0f)
	, bulletSpawnParentLocation_(nullptr)
	, bulletSpawnLocation_(nullptr)
	, bulletDirection_(float4::RIGHT)
	, bulletRotation_(float4::ZERO)
	, timeCounter_(0.0f)
	, bParryJump_(false)
	, hp_(3)
	, superMeterCount_(0)
	, dustSpawnDelay_(0.0f)
	, superMeterGauge_(0)
{

}

Player::~Player()
{

}

void Player::Start()
{

	fireLoopSound_ = std::make_unique<GameEngineSoundPlayer>("sfx_player_default_fire_loop_01.wav");

	bulletSpawnParentLocation_ = CreateTransformComponent<GameEngineTransformComponent>(transform_);
	bulletSpawnParentLocation_->SetScale(1.0f);
	bulletSpawnLocation_ = CreateTransformComponent<GameEngineTransformComponent>(bulletSpawnParentLocation_);

	initInput();
	initRendererAndAnimation();
	initState();
	initCollision();
}

void Player::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (state_.GetCurrentStateName() == "CinematicState")
	{
		return;
	}

	//GameEngineDebug::OutPutDebugString(normalState_.GetCurrentStateName() + "\n");

	// Peashot Straight로 발사할 때 위아래 왔다갔다
	{
		static const char* tempString = "Shoot_Straight";
		static const char* tempString2 = "Run_Shoot_Straight";
		static const char* tempString3 = "Duck_Shoot";
		static float bulletPositionY = 0.0f;

		const char* str = renderer_->GetCurrentAnimation()->Name_.c_str();

		if (!strcmp(tempString, str) || !strcmp(tempString2, str) || !strcmp(tempString3, str))
		{
			bulletPositionY += _deltaTime * 10.f;
			float a = sinf(bulletPositionY) * 20.f;
			bulletSpawnLocation_->SetLocationY(a + 45.f);
		}
		else
		{
			bulletSpawnLocation_->SetLocationY(30.0f);
		}
	}

	// 무적, 깜빡임처리
	if (bInvincible_)
	{
		invincibleTime_ += _deltaTime;
		blinkTime_ += _deltaTime;

		if (blinkTime_ > BLINK_DELAY)
		{
			bBlink_ = !bBlink_;
			blinkTime_ = 0.0f;
		}

		if (invincibleTime_ > INVINCIBLE_TIME)
		{
			bInvincible_ = false;
			bBlink_ = false;
		}
	}

	if (bBlink_)
	{
		renderer_->SetColor({ 1.0f, 1.0f, 1.0f, 0.2f });
	}
	else
	{
		renderer_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	if (bLeft_)
	{
		renderer_->SetFlip(true, false);
	}
	else
	{
		renderer_->SetFlip(false, false);
	}

	if (hp_ >= 0 && hp_ < 4)
	{
		hpRenderer_->ChangeAnimation("HP" + std::to_string(hp_));
	}

	for (GameEngineImageRenderer* r : superMeters_)
	{
		r->Off();
	}

	for (size_t i = 0; i < superMeterCount_; i++)
	{
		superMeters_[i]->On();
	}
}

std::string Player::GetNormalState()
{
	return normalState_.GetCurrentStateName();
}

std::string Player::GetParentState()
{
	return state_.GetCurrentStateName();
}

void Player::Damage()
{
	normalState_ << "Jump";
	fireLoopSound_->Stop();
	fireStartRenderer_->Off();
	bGround_ = false;
	state_ << "DamagedState";
}

void Player::ParryJump()
{
	bGround_ = true;
	normalState_ << "Jump";
}

void Player::SetStateNormal()
{
	state_ << "NormalState";
}

void Player::SetStateCinematic()
{
	state_ << "CinematicState";
}

void Player::SetCineState(const std::string& _cinematicStateName)
{
	cinematicState_ << _cinematicStateName;
}

void Player::SetInvincible(float _invincibleTime)
{
	bInvincible_ = true;
	invincibleTime_ = _invincibleTime;
}

void Player::AddSuperGauge()
{
	superMeterGauge_++;

	if (superMeterGauge_ >= 20)
	{
		superMeterGauge_ = 0;
		if (superMeterCount_ < 5)
		{
			superMeterCount_++;
		}
		GameEngineRandom random;
		int soundNumber = random.RandomInt(1, 2);
		std::string soundName = "sfx_player_parry_power_increment_p" + std::to_string(soundNumber) + ".wav";
		GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
	}
}

void Player::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());

	renderer_->CreateAnimationFolder("Intro", 0.0416f, false);
	renderer_->CreateAnimationFolder("Idle", "Idle", 0.04f);
	renderer_->CreateAnimationFolder("Run", "Run", 0.033f);
	renderer_->CreateAnimationFolder("Air", 0.034f);
	renderer_->CreateAnimationFolder("Parry", 0.033f, false);

	renderer_->CreateAnimationFolder("Aim_Straight");
	renderer_->CreateAnimationFolder("Aim_Up");
	renderer_->CreateAnimationFolder("Aim_Down");
	renderer_->CreateAnimationFolder("Aim_DiagonalUp");
	renderer_->CreateAnimationFolder("Aim_DiagonalDown");

	renderer_->CreateAnimationFolder("Duck", "Duck", 0.033f, false);
	renderer_->CreateAnimationFolder("DuckIdle", "DuckIdle", 0.04f);

	renderer_->CreateAnimationFolder("Dash", 0.034f, false);
	renderer_->CreateAnimationFolder("Dash_Air", 0.034f, false);

	renderer_->CreateAnimationFolder("Run_Shoot_Straight");
	renderer_->CreateAnimationFolder("Run_Shoot_DiagonalUp");

	renderer_->CreateAnimationFolder("Shoot_Straight");
	renderer_->CreateAnimationFolder("Shoot_Up");
	renderer_->CreateAnimationFolder("Shoot_Down");
	renderer_->CreateAnimationFolder("Shoot_DiagonalUp");
	renderer_->CreateAnimationFolder("Shoot_DiagonalDown");

	renderer_->CreateAnimationFolder("SSAir_Straight", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSAir_Up", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSAir_Down", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSAir_DiagonalUp", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSAir_DiagonalDown", 0.0416f, false);

	renderer_->CreateAnimationFolder("SSGround_Straight", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSGround_Up", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSGround_Down", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSGround_DiagonalUp", 0.0416f, false);
	renderer_->CreateAnimationFolder("SSGround_DiagonalDown", 0.0416f, false);

	renderer_->CreateAnimationFolder("Duck_Shoot");

	renderer_->CreateAnimationFolder("Hit_Ground", 0.05f, false);

	renderer_->CreateAnimationFolder("Scared", 0.034f, false);

	renderer_->ChangeAnimation("Idle");

	fireStartRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(bulletSpawnParentLocation_);
	fireStartRenderer_->CreateAnimationFolder("Peashot_Spawn", 0.034f, true, true);
	fireStartRenderer_->ChangeAnimation("Peashot_Spawn");
	fireStartRenderer_->SetLocationZ(1.f);
	fireStartRenderer_->Off();

	GameEngineTransformComponent* hpTransform = CreateTransformComponent<GameEngineTransformComponent>(level_->GetMainCameraActor()->GetTransform());
	hpTransform->SetLocation(-620.f, -345.f, -40.0f);
	hpRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(hpTransform);
	hpRenderer_->CreateAnimationFolder("HP0", 0.0416f, false);
	hpRenderer_->CreateAnimationFolder("HP1", 0.12f);
	hpRenderer_->CreateAnimationFolder("HP2", 0.0416f, false);
	hpRenderer_->CreateAnimationFolder("HP3", 0.0416f, false);
	hpRenderer_->SetPivot(eImagePivot::BOTTOM_LEFT);

	hpRenderer_->ChangeAnimation("HP3");

	for (size_t i = 0; i < 5; i++)
	{
		GameEngineImageRenderer* sp = CreateTransformComponent<GameEngineImageRenderer>(hpTransform);
		sp->CreateAnimationFolder("SuperMeter", 1.0f);
		sp->ChangeAnimation("SuperMeter");
		sp->SetLocationX(95.f + 19 * i);
		sp->Off();

		superMeters_.push_back(sp);
	}
}

void Player::initInput()
{
	GameEngineInput::GetInstance().CreateKey("Left", VK_LEFT);
	GameEngineInput::GetInstance().CreateKey("Right", VK_RIGHT);
	GameEngineInput::GetInstance().CreateKey("Up", VK_UP);
	GameEngineInput::GetInstance().CreateKey("Down", VK_DOWN);
	GameEngineInput::GetInstance().CreateKey("Q", 'Q');
	GameEngineInput::GetInstance().CreateKey("E", 'E');
	GameEngineInput::GetInstance().CreateKey("Z", 'Z');
	GameEngineInput::GetInstance().CreateKey("C", 'C');
	GameEngineInput::GetInstance().CreateKey("X", 'X');
	GameEngineInput::GetInstance().CreateKey("V", 'V');
	GameEngineInput::GetInstance().CreateKey("LShift", VK_LSHIFT);

	GameEngineInput::GetInstance().CreateKey("P", 'P');
}

void Player::initCollision()
{
	collider_ = CreateTransformComponent<GameEngineCollision>();
	collider_->SetCollisionType(eCollisionType::Rect);
	collider_->SetScale(70.f);
	collider_->SetLocationY(50.f);

	bottomCenterCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	bottomCenterCollision_->SetCollisionType(eCollisionType::Rect);
	bottomCenterCollision_->SetLocation(0.0f, 3.0f, 0.0f);
	//bottomCenterCollision_->SetScale(2.0f);

	groundCheckCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	groundCheckCollision_->SetCollisionType(eCollisionType::Rect);
	groundCheckCollision_->SetLocation(0.0f, 0.0f, 0.0f);
	groundCheckCollision_->SetScale(2.0f);

	fallCheckCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	fallCheckCollision_->SetCollisionType(eCollisionType::Rect);
	fallCheckCollision_->SetLocation(0.0f, -10.0f, 0.0f);
	fallCheckCollision_->SetScale(2.0f);

	leftSideCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	leftSideCollision_->SetCollisionType(eCollisionType::Rect);
	leftSideCollision_->SetLocation(-50.0f, 10.0f, 0.0f);
	//leftSideCollision_->SetScale(2.0f);

	rightSideCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	rightSideCollision_->SetCollisionType(eCollisionType::Rect);
	rightSideCollision_->SetLocation(50.0f, 10.0f, 0.0f);
	//rightSideCollision_->SetScale(2.0f);

	//headCollision_ = CreateTransformComponent<GameEngineCollision>(transform_);
	//headCollision_->SetCollisionType(eCollisionType::Rect);
	//headCollision_->SetLocation(0.0f, 145.0f, 0.0f);
	//headCollision_->SetScale(2.0f);

//#ifdef _DEBUG
//	GameEngineRenderer* newRenderer;
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(bottomCenterCollision_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(groundCheckCollision_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(leftSideCollision_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(rightSideCollision_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(headCollision_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-1.0f);
//
//	newRenderer = CreateTransformComponent<GameEngineRenderer>(collider_);
//	newRenderer->SetRenderingPipeline("DebugRect");
//	newRenderer->SetLocationZ(-2.0f);
//	newRenderer->SetScale(1.05f);
//#endif
}

void Player::initState()
{
	state_.CreateState("NormalState", std::bind(&Player::startNormalState, this, std::placeholders::_1), std::bind(&Player::updateNormalState, this, std::placeholders::_1));
	state_.CreateState("ShootState", std::bind(&Player::startShootState, this, std::placeholders::_1), std::bind(&Player::updateShootState, this, std::placeholders::_1));
	state_.CreateState("DamagedState", std::bind(&Player::startDamagedState, this, std::placeholders::_1), std::bind(&Player::updateDamagedState, this, std::placeholders::_1));
	state_.CreateState("FallDamagedState", std::bind(&Player::startFallDamagedState, this, std::placeholders::_1), std::bind(&Player::updateFallDamagedState, this, std::placeholders::_1));
	state_.CreateState("CinematicState", std::bind(&Player::startCinematicState, this, std::placeholders::_1), std::bind(&Player::updateCinematicState, this, std::placeholders::_1));

	normalState_.CreateState("Idle", std::bind(&Player::startIdle, this, std::placeholders::_1), std::bind(&Player::updateIdle, this, std::placeholders::_1));
	normalState_.CreateState("Run", std::bind(&Player::startRun, this, std::placeholders::_1), std::bind(&Player::updateRun, this, std::placeholders::_1));
	normalState_.CreateState("Jump", std::bind(&Player::startJump, this, std::placeholders::_1), std::bind(&Player::updateJump, this, std::placeholders::_1));
	normalState_.CreateState("DownJump", std::bind(&Player::startDownJump, this, std::placeholders::_1), std::bind(&Player::updateDownJump, this, std::placeholders::_1));
	normalState_.CreateState("Damaged", std::bind(&Player::startDamaged, this, std::placeholders::_1), std::bind(&Player::updateDamaged, this, std::placeholders::_1));
	normalState_.CreateState("Dash", std::bind(&Player::startDash, this, std::placeholders::_1), std::bind(&Player::updateDash, this, std::placeholders::_1));
	normalState_.CreateState("Duck", std::bind(&Player::startDuck, this, std::placeholders::_1), std::bind(&Player::updateDuck, this, std::placeholders::_1));
	normalState_.CreateState("DuckIdle", std::bind(&Player::startDuckIdle, this, std::placeholders::_1), std::bind(&Player::updateDuckIdle, this, std::placeholders::_1));
	normalState_.CreateState("StandUp", std::bind(&Player::startStandUp, this, std::placeholders::_1), std::bind(&Player::updateStandUp, this, std::placeholders::_1));
	normalState_.CreateState("Shoot", std::bind(&Player::startShoot, this, std::placeholders::_1), std::bind(&Player::updateShoot, this, std::placeholders::_1));
	normalState_.CreateState("Lock", std::bind(&Player::startLock, this, std::placeholders::_1), std::bind(&Player::updateLock, this, std::placeholders::_1));
	normalState_.CreateState("LockedShot", std::bind(&Player::startLockedShot, this, std::placeholders::_1), std::bind(&Player::updateLockedShot, this, std::placeholders::_1));
	normalState_.CreateState("ShootWhileDucking", std::bind(&Player::startShootWhileDucking, this, std::placeholders::_1), std::bind(&Player::updateShootWhileDucking, this, std::placeholders::_1));
	normalState_.CreateState("ShootWhileRunning", std::bind(&Player::startShootWhileRunning, this, std::placeholders::_1), std::bind(&Player::updateShootWhileRunning, this, std::placeholders::_1));
	normalState_.CreateState("Parry", std::bind(&Player::startParry, this, std::placeholders::_1), std::bind(&Player::updateParry, this, std::placeholders::_1), std::bind(&Player::endParry, this, std::placeholders::_1));
	normalState_.CreateState(MakeState(Player, EX));

	cinematicState_.CreateState("Idle", std::bind(&Player::startCinematicIdle, this, std::placeholders::_1), std::bind(&Player::updateCinematicIdle, this, std::placeholders::_1));
	cinematicState_.CreateState("DevilPhaseOneEndFalling", std::bind(&Player::startDevilPhaseOneEndFalling, this, std::placeholders::_1), std::bind(&Player::updateDevilPhaseOneEndFalling, this, std::placeholders::_1));
	cinematicState_.CreateState("Scared", std::bind(&Player::startScared, this, std::placeholders::_1), std::bind(&Player::updateScared, this, std::placeholders::_1));
	cinematicState_.CreateState("ScaredWait", std::bind(&Player::startScaredWait, this, std::placeholders::_1), std::bind(&Player::updateScaredWait, this, std::placeholders::_1));

	normalState_.ChangeState("Idle");
	state_.ChangeState("NormalState");
}

void Player::addGravity(float _deltaTime)
{
	if (float4::BLACK != Map::GetColor(groundCheckCollision_) && float4::BLUE != Map::GetColor(groundCheckCollision_)
		&& nullptr == groundCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
	}
	else
	{
		gravitySpeed_ = 0.f;
		bGround_ = true;
		bCanDash_ = true;
		bCanJump_ = true;
		transform_->SetLocationY(static_cast<float>(static_cast<int>(transform_->GetLocation().y)));
	}

	while (float4::BLACK == Map::GetColor(bottomCenterCollision_) || float4::BLUE == Map::GetColor(bottomCenterCollision_)
		|| nullptr != bottomCenterCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		transform_->AddLocation(0.0f, 0.5f);
		float4 location = transform_->GetWorldLocation();
		transform_->SetLocation(static_cast<float>(location.ix()), static_cast<float>(location.iy()));
	}
}

void Player::playHitSound()
{
	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 5);
	std::string soundName = "sfx_player_hit_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void Player::playJumpSound()
{
	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 3);
	std::string soundName = "sfx_player_jump_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void Player::playLandingSound()
{
	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 1);
	std::string soundName = "sfx_player_land_ground_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void Player::playParrySound()
{
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_parry_slap_01.wav");

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 2);
	std::string soundName = "sfx_player_parry_power_up_hit_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

	if (superMeterCount_ >= 5)
	{
		GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_parry_slap_01.wav");
	}
}

void Player::startNormalState(float _deltaTime)
{
}

void Player::updateNormalState(float _deltaTime)
{
	if (!bInvincible_ &&
		(nullptr != collider_->IsCollideOne(eCollisionGroup::Monster)
			|| nullptr != collider_->IsCollideOne(eCollisionGroup::MonsterProjectile))
		)
	{
		normalState_ << "Jump";
		bGround_ = false;
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
		state_ << "DamagedState";
		return;
	}

	if (!bInvincible_ && nullptr != collider_->IsCollideOne(eCollisionGroup::ParryMonster))
	{
		if (GetNormalState() != "Parry")
		{
			normalState_ << "Jump";
			bGround_ = false;
			fireLoopSound_->Stop();
			fireStartRenderer_->Off();
			state_ << "DamagedState";
			return;
		}
	}

	if (float4::RED == Map::GetColor(groundCheckCollision_))
	{
		if (bInvincible_)
		{
			bGround_ = true;
			normalState_ << "Jump";
		}
		else
		{
			fireLoopSound_->Stop();
			fireStartRenderer_->Off();
			normalState_ << "Idle";
			state_ << "FallDamagedState";
			return;
		}
	}

	if (GameEngineInput::GetInstance().IsKeyPress("X") && normalState_.GetCurrentStateName() != "Dash" && normalState_.GetCurrentStateName() != "EX")
	{
		state_ << "ShootState";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("V") && superMeterCount_ > 0
		&& normalState_.GetCurrentStateName() != "Dash" && normalState_.GetCurrentStateName() != "EX")
	{
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
		superMeterCount_--;
		normalState_ << "EX";
	}

	normalState_.Update(_deltaTime);
}

void Player::startShootState(float _deltaTime)
{
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_default_fire_start_01.wav");
	fireStartRenderer_->ChangeAnimation("Peashot_Spawn");
	fireStartRenderer_->On();
	shootDelay_ = 0.0f;
}

void Player::updateShootState(float _deltaTime)
{
	if (!bInvincible_ &&
		(nullptr != collider_->IsCollideOne(eCollisionGroup::Monster) || nullptr != collider_->IsCollideOne(eCollisionGroup::MonsterProjectile))
		)
	{
		state_ << "DamagedState";
		normalState_ << "Jump";
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
		bGround_ = false;
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("X")
		|| normalState_.GetCurrentStateName() == "Dash")
	{
		state_ << "NormalState";
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
		return;
	}

	if (float4::RED == Map::GetColor(groundCheckCollision_))
	{
		if (bInvincible_)
		{
			bGround_ = true;
			normalState_ << "Jump";
		}
		else
		{
			fireLoopSound_->Stop();
			fireStartRenderer_->Off();
			normalState_ << "Idle";
			state_ << "FallDamagedState";
			return;
		}
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Down")
		&& normalState_.GetCurrentStateName() == "LockedShot")
	{
		fireStartRenderer_->SetLocationZ(-1.f);
	}
	else
	{
		fireStartRenderer_->SetLocationZ(0.5f);
	}

	if (!fireLoopSound_->IsPlaying())
	{
		fireLoopSound_->Play();
	}

	shootDelay_ -= _deltaTime;
	if (shootDelay_ < 0.0f)
	{
		Peashot* newShot = level_->CreateActor<Peashot>("Pea");
		newShot->GetTransform()->SetLocation(bulletSpawnLocation_->GetWorldLocation());
		newShot->InitBullet(bLeft_, bulletDirection_, bulletRotation_, this);
		shootDelay_ = SHOOT_DELAY;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("V") && superMeterCount_ > 0
		&& normalState_.GetCurrentStateName() != "Dash" && normalState_.GetCurrentStateName() != "EX")
	{
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
		superMeterCount_--;
		normalState_ << "EX";
		state_ << "NormalState";
	}

	normalState_.Update(_deltaTime);
}

void Player::startDamagedState(float _deltaTime)
{
	renderer_->ChangeAnimation("Hit_Ground");

	playHitSound();
	hp_--;

	level_->CreateActor<HitFX>()->GetTransform()->SetLocation(transform_->GetWorldLocation());
}

void Player::updateDamagedState(float _deltaTime)
{
	if (!GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		bInvincible_ = true;
		bBlink_ = false;
		invincibleTime_ = 0.0f;
		blinkTime_ = 0.0f;

		state_ << "NormalState";
		return;
	}
}

void Player::startFallDamagedState(float _deltaTime)
{
	renderer_->ChangeAnimation("Hit_Ground");
	playHitSound();
}

void Player::updateFallDamagedState(float _deltaTime)
{
	if (!GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		fireLoopSound_->Stop();
		fireStartRenderer_->Off();
	}

	transform_->AddLocation(0.0f, 850.f * _deltaTime);

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}

		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}

		bLeft_ = false;
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		bInvincible_ = true;
		bBlink_ = false;
		invincibleTime_ = 0.0f;
		blinkTime_ = 0.0f;

		bGround_ = true;

		state_ << "NormalState";
		normalState_ << "Jump";
		return;
	}
}

void Player::startCinematicState(float _deltaTime)
{

}

void Player::updateCinematicState(float _deltaTime)
{
	cinematicState_.Update(_deltaTime);
}

void Player::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Idle");
	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;

	renderer_->SetLocationX(0.0f);
}

void Player::updateIdle(float _deltaTime)
{
	addGravity(_deltaTime);

	if (float4::BLACK != Map::GetColor(fallCheckCollision_) && float4::BLUE != Map::GetColor(fallCheckCollision_)
		&& nullptr == fallCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		bGround_ = false;
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "Shoot";
		return;
	}


	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
		normalState_ << "Run";
		return;
	}
	if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bLeft_ = false;
		normalState_ << "Run";
		return;
	}


	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "Lock";
		return;
	}



	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "Duck";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
			bulletDirection_ = float4::UP;
			bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
			bulletDirection_ = float4::UP;
			bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
		}
		renderer_->ChangeAnimation("Aim_Up");
	}
	else
	{
		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_ = float4::ZERO;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_ = float4::ZERO;
		}
		renderer_->ChangeAnimation("Idle");
	}
}

void Player::startRun(float _deltaTime)
{
	renderer_->ChangeAnimation("Run");
}

void Player::updateRun(float _deltaTime)
{
	addGravity(_deltaTime);

	dustSpawnDelay_ -= _deltaTime;
	if (dustSpawnDelay_ < 0.0f && (renderer_->GetCurrentAnimation()->CurFrame_ == 5 || renderer_->GetCurrentAnimation()->CurFrame_ == 15))
	{
		level_->CreateActor<Dust>()->GetTransform()->SetLocation(transform_->GetWorldLocation());
		dustSpawnDelay_ = DUST_SPAWN_DLEAY;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "Duck";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "ShootWhileRunning";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "Lock";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
		bulletRotation_ = float4::ZERO;
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
		bulletRotation_ = float4::ZERO;
		bLeft_ = false;
	}
	else
	{
		normalState_ << "Idle";
		return;
	}

	if (float4::BLACK != Map::GetColor(fallCheckCollision_) && float4::BLUE != Map::GetColor(fallCheckCollision_)
		&& nullptr == fallCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		bGround_ = false;
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}

	//if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	//{
	//	transform_->AddLocation(0.0f, MOVE_SPEED * _deltaTime);
	//}
	//else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	//{
	//	transform_->AddLocation(0.0f, -MOVE_SPEED * _deltaTime);
	//}
	//if (GameEngineInput::GetInstance().IsKeyFree("Left") && GameEngineInput::GetInstance().IsKeyFree("Right")&&
	//	GameEngineInput::GetInstance().IsKeyFree("Up") && GameEngineInput::GetInstance().IsKeyFree("Down"))
	//{
	//	normalState_ << "Idle";
	//}
}

void Player::startJump(float _deltaTime)
{
	if (bGround_)
	{
		bCanJump_ = true;
		jumpTime_ = 0.2f;
		gravitySpeed_ = JUMP_POWER;
	}
	else
	{
		bCanJump_ = false;
	}


	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;

	if (bParryJump_)
	{
		bParryJump_ = false;
		return;
	}
	renderer_->ChangeAnimation("Air");
}

void Player::updateJump(float _deltaTime)
{
	bGround_ = false;

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("Air");
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		normalState_ << "Parry";
		return;
	}

	if (bCanJump_)
	{
		jumpTime_ -= _deltaTime;
		transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
		if (jumpTime_ > 0.1f)
		{

		}
		else if (true == GameEngineInput::GetInstance().IsKeyFree("Z") || jumpTime_ < 0.0f)
		{
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
			bCanJump_ = false;
		}
	}
	else
	{
		if (gravitySpeed_ <= 0.0f)
		{
			if (float4::BLACK == Map::GetColor(bottomCenterCollision_) || float4::BLUE == Map::GetColor(bottomCenterCollision_)
				|| nullptr != bottomCenterCollision_->IsCollideOne(eCollisionGroup::Platform))
			{
				gravitySpeed_ = 0.0f;
				bGround_ = true;
				bCanJump_ = true;
				bCanDash_ = true;
				playLandingSound();
				level_->CreateActor<LandDust>()->GetTransform()->SetLocation(transform_->GetWorldLocation());
				normalState_ << "Idle";
			}
			else
			{
				bGround_ = false;
				transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
				gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
			}
		}
		else
		{
			bGround_ = false;
			transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		}

		//if (float4::BLACK != Map::GetColor(groundCheckCollision_) && float4::BLUE != Map::GetColor(groundCheckCollision_)
		//	&& nullptr == groundCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
		//{
		//	bGround_ = false;
		//	transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
		//	gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		//}
		//else if ()
		//{
		//	bGround_ = true;
		//	bCanJump_ = true;
		//	bCanDash_ = true;
		//	normalState_ << "Idle";
		//	return;
		//}
	}

	// 총알 쏘는 방향
	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { -0.5f, 0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { 0.5f, 0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}
		}
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, 0.0f);
			bulletDirection_ = { -0.5f, -0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, 0.0f);
			bulletDirection_ = { 0.5f, -0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}
		}
	}
	else
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_.z = 0.0f;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_.z = 0.0f;
		}
	}

	// 움직임
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = false;
	}
}

void Player::startDownJump(float _deltaTime)
{
	bGround_ = false;
	bCanJump_ = false;
	bDownJump_ = true;
	renderer_->ChangeAnimation("Air");
}

void Player::updateDownJump(float _deltaTime)
{
	bGround_ = false;

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (float4::BLUE != Map::GetColor(groundCheckCollision_)
		&& nullptr == groundCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		bDownJump_ = false;
	}

	if (bDownJump_)
	{
		transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
		gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
	}
	else
	{
		if (float4::BLACK != Map::GetColor(groundCheckCollision_) && float4::BLUE != Map::GetColor(groundCheckCollision_)
			&& nullptr == groundCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
		{
			bGround_ = false;
			transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		}
		else
		{
			bGround_ = true;
			bCanJump_ = true;
			bCanDash_ = true;
			playLandingSound();
			level_->CreateActor<LandDust>()->GetTransform()->SetLocation(transform_->GetWorldLocation());
			normalState_ << "Idle";
			return;
		}
	}

	// 움직임
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			if (float4::BLACK != Map::GetColor(leftSideCollision_))
			{
				transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
			}
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			if (float4::BLACK != Map::GetColor(rightSideCollision_))
			{
				transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
			}
			bLeft_ = false;
		}
	}
}

void Player::startDamaged(float _deltaTime)
{
}

void Player::updateDamaged(float _deltaTime)
{
}

void Player::startDash(float _deltaTime)
{
	fireLoopSound_->Stop();
	renderer_->ChangeAnimation("Dash", true);
	gravitySpeed_ = 0.0f;
	bCanDash_ = false;

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 3);
	std::string soundName = "sfx_player_dash_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);

	level_->CreateActor<DashDust>()->GetTransform()->SetLocation(transform_->GetWorldLocation());
}

void Player::updateDash(float _deltaTime)
{
	shootDelay_ = 0.1f;
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		normalState_ << "Idle";
		return;
	}

	if (bLeft_)
	{
		renderer_->SetLocationX(65.f);
		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-DASH_SPEED * _deltaTime, 0.0f);
		}
		//GameEngineDebug::OutPutDebugString(std::to_string(renderer_->GetWorldLocation().x) + "\n");
	}
	else
	{
		renderer_->SetLocationX(-65.f);
		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(DASH_SPEED * _deltaTime, 0.0f);
		}
	}
}

void Player::startDuck(float _deltaTime)
{
	renderer_->ChangeAnimation("Duck", true);
}

void Player::updateDuck(float _deltaTime)
{
	addGravity(_deltaTime);
	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		if (float4::BLUE == Map::GetColor(fallCheckCollision_)
			|| nullptr != fallCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
		{
			bGround_ = false;
			transform_->AddLocation(0.0f, -5.0f);
			normalState_ << "DownJump";
		}
		else
		{
			playJumpSound();
			normalState_ << "Jump";
		}
		return;
	}
	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}
	if (!GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "StandUp";
		return;
	}
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		normalState_ << "DuckIdle";
		return;
	}
}

void Player::startDuckIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("DuckIdle", true);
}

void Player::updateDuckIdle(float _deltaTime)
{
	addGravity(_deltaTime);

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		if (float4::BLUE == Map::GetColor(fallCheckCollision_)
			|| nullptr != fallCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
		{
			bGround_ = false;
			transform_->AddLocation(0.0f, -5.0f);
			normalState_ << "DownJump";
		}
		else
		{
			playJumpSound();
			normalState_ << "Jump";
		}
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "Lock";
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "StandUp";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "ShootWhileDucking";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DUCK_OFFSET);
		bulletDirection_ = float4::LEFT;
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DUCK_OFFSET);
		bulletDirection_ = float4::RIGHT;
		bLeft_ = false;
	}
}

void Player::startStandUp(float _deltaTime)
{
	renderer_->ChangeAnimation("Duck", true);
}

void Player::updateStandUp(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}
	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}
	if (1 == renderer_->GetCurrentAnimation()->CurFrame_)
	{
		normalState_ << "Idle";
		return;
	}
}

void Player::startShoot(float _deltaTime)
{
	renderer_->ChangeAnimation("Shoot_Straight");

	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;
}

void Player::updateShoot(float _deltaTime)
{
	addGravity(_deltaTime);
	if (float4::BLACK != Map::GetColor(fallCheckCollision_) && float4::BLUE != Map::GetColor(fallCheckCollision_)
		&& nullptr == fallCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		bGround_ = false;
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "Idle";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
			bulletDirection_ = float4::UP;
			bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
			bulletDirection_ = float4::UP;
			bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
		}
		renderer_->ChangeAnimation("Shoot_Up");
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "ShootWhileDucking";
		return;
	}
	else
	{
		renderer_->ChangeAnimation("Shoot_Straight");
		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_ = float4::ZERO;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_ = float4::ZERO;
		}
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
		normalState_ << "ShootWhileRunning";
		return;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bLeft_ = false;
		normalState_ << "ShootWhileRunning";
		return;
	}
}

void Player::startLock(float _deltaTime)
{
	renderer_->ChangeAnimation("Aim_Straight");
}

void Player::updateLock(float _deltaTime)
{
	addGravity(_deltaTime);

	if (!GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "Idle";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "LockedShot";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { -0.5f, 0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Aim_DiagonalUp");
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { 0.5f, 0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Aim_DiagonalUp");
			bLeft_ = false;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}
			renderer_->ChangeAnimation("Aim_Up");
		}
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, 0.0f);
			bulletDirection_ = { -0.5f, -0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Aim_DiagonalDown");
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, 0.0f);
			bulletDirection_ = { 0.5f, -0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Aim_DiagonalDown");
			bLeft_ = false;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}
			renderer_->ChangeAnimation("Aim_Down");
		}
	}
	else
	{
		renderer_->ChangeAnimation("Aim_Straight");
		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_ = float4::ZERO;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_ = float4::ZERO;
		}
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bLeft_ = false;
	}
}

void Player::startLockedShot(float _deltaTime)
{
	renderer_->ChangeAnimation("Shoot_Straight");
	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;
}

void Player::updateLockedShot(float _deltaTime)
{
	addGravity(_deltaTime);

	if (!GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "Shoot";
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "Lock";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { -0.5f, 0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Shoot_DiagonalUp");
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { 0.5f, 0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Shoot_DiagonalUp");
			bLeft_ = false;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}

			renderer_->ChangeAnimation("Shoot_Up");
		}
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, 0.0f);
			bulletDirection_ = { -0.5f, -0.4f };
			bulletRotation_.z = 40.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Shoot_DiagonalDown");
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, 0.0f);
			bulletDirection_ = { 0.5f, -0.4f };
			bulletRotation_.z = -40.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Shoot_DiagonalDown");
			bLeft_ = false;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}

			renderer_->ChangeAnimation("Shoot_Down");
		}
	}
	else
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bLeft_ = false;
		}
		renderer_->ChangeAnimation("Shoot_Straight");
		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_ = float4::ZERO;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_ = float4::ZERO;
		}
	}
}

void Player::startShootWhileDucking(float _deltaTime)
{
	renderer_->ChangeAnimation("Duck_Shoot");

	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DUCK_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DUCK_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;
}

void Player::updateShootWhileDucking(float _deltaTime)
{
	addGravity(_deltaTime);

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		if (float4::BLUE == Map::GetColor(groundCheckCollision_)
			|| nullptr != groundCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
		{
			bGround_ = false;
			normalState_ << "DownJump";
		}
		else
		{
			playJumpSound();
			normalState_ << "Jump";
		}
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "DuckIdle";
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "StandUp";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "LockedShot";
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "Shoot";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DUCK_OFFSET);
		bulletDirection_ = float4::LEFT;
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DUCK_OFFSET);
		bulletDirection_ = float4::RIGHT;
		bLeft_ = false;
	}
}

void Player::startShootWhileRunning(float _deltaTime)
{
	renderer_->ChangeAnimation("Run_Shoot_Straight");

	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;
}

void Player::updateShootWhileRunning(float _deltaTime)
{
	addGravity(_deltaTime);

	dustSpawnDelay_ -= _deltaTime;
	if (dustSpawnDelay_ < 0.0f && (renderer_->GetCurrentAnimation()->CurFrame_ == 5 || renderer_->GetCurrentAnimation()->CurFrame_ == 15))
	{
		level_->CreateActor<Dust>()->GetTransform()->SetLocation(transform_->GetWorldLocation());
		dustSpawnDelay_ = DUST_SPAWN_DLEAY;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (!GameEngineInput::GetInstance().IsKeyPress("X"))
	{
		normalState_ << "Run";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("C"))
	{
		normalState_ << "LockedShot";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		normalState_ << "ShootWhileDucking";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Up"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { -0.5f, 0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Run_Shoot_DiagonalUp");
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_ = float4::ZERO;
			renderer_->ChangeAnimation("Run_Shoot_Straight");
		}

		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Up"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { 0.5f, 0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
			renderer_->ChangeAnimation("Run_Shoot_DiagonalUp");
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_ = float4::ZERO;
			renderer_->ChangeAnimation("Run_Shoot_Straight");
		}

		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = false;
	}
	else
	{
		normalState_ << "Shoot";
		return;
	}

	if (float4::BLACK != Map::GetColor(fallCheckCollision_) && float4::BLUE != Map::GetColor(fallCheckCollision_)
		&& nullptr == fallCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
	{
		bGround_ = false;
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("Z"))
	{
		playJumpSound();
		normalState_ << "Jump";
		return;
	}
}

void Player::startParry(float _deltaTime)
{
	if (bGround_)
	{
		bCanJump_ = true;
		jumpTime_ = 0.2f;
		gravitySpeed_ = JUMP_POWER;
		transform_->AddLocation(0.0f, 10.0f);
	}
	else
	{
		bCanJump_ = false;
	}


	if (bLeft_)
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::LEFT;
	}
	else
	{
		bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
		bulletDirection_ = float4::RIGHT;
	}
	bulletRotation_ = float4::ZERO;

	collider_->SetScale(150.f);

	renderer_->ChangeAnimation("Parry");
}

void Player::updateParry(float _deltaTime)
{
	bGround_ = false;

	GameEngineCollision* parryCollision = collider_->IsCollideOne(eCollisionGroup::ParryObject);
	if (nullptr != parryCollision)
	{
		ParryObjectBase* parryObject = dynamic_cast<ParryObjectBase*>(parryCollision->GetActor());
		if (nullptr != parryObject)
		{
			parryObject->SetParryable(false);
			parryObject->Parry();

			level_->SetBulletTime(0.01f, 0.2f);

			GameEngineActor* parryEffect = level_->CreateActor<ParryEffect>();
			float4 parryEffectLocation = parryObject->GetTransform()->GetWorldLocation() - transform_->GetWorldLocation();
			parryEffectLocation = transform_->GetWorldLocation() + (parryEffectLocation.operator/({ 2.0f, 2.0f, 2.0f, 2.0f }));
			parryEffect->GetTransform()->SetLocation(parryEffectLocation);

			SetInvincible(1.0f);

			if (superMeterCount_ < 5)
			{
				superMeterCount_++;
			}

			playParrySound();

			bGround_ = true;
			bParryJump_ = true;
			normalState_ << "Jump";
			return;
		}
	}

	parryCollision = collider_->IsCollideOne(eCollisionGroup::ParryMonster);
	if (nullptr != parryCollision)
	{
		ParryObjectBase* parryObject = dynamic_cast<ParryObjectBase*>(parryCollision->GetActor());
		if (nullptr != parryObject)
		{
			parryObject->SetParryable(false);
			parryObject->Parry();

			level_->SetBulletTime(0.01f, 0.2f);

			GameEngineActor* parryEffect = level_->CreateActor<ParryEffect>();
			float4 parryEffectLocation = parryObject->GetTransform()->GetWorldLocation() - transform_->GetWorldLocation();
			parryEffectLocation = transform_->GetWorldLocation() + (parryEffectLocation.operator/({ 2.0f, 2.0f, 2.0f, 2.0f }));
			parryEffect->GetTransform()->SetLocation(parryEffectLocation);

			SetInvincible(1.0f);
			if (superMeterCount_ < 5)
			{
				superMeterCount_++;
			}

			playParrySound();

			bGround_ = true;
			bParryJump_ = true;
			normalState_ << "Jump";
			return;
		}
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		normalState_ << "Jump";
		return;
	}

	if (GameEngineInput::GetInstance().IsKeyDown("LShift") && bCanDash_)
	{
		normalState_ << "Dash";
		return;
	}

	if (bCanJump_)
	{
		jumpTime_ -= _deltaTime;
		transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
		if (jumpTime_ > 0.1f)
		{

		}
		else if (true == GameEngineInput::GetInstance().IsKeyFree("Z") || jumpTime_ < 0.0f)
		{
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
			bCanJump_ = false;
		}
	}
	else
	{
		if (gravitySpeed_ <= 0.0f)
		{
			if (float4::BLACK == Map::GetColor(groundCheckCollision_) || float4::BLUE == Map::GetColor(groundCheckCollision_)
				|| nullptr != groundCheckCollision_->IsCollideOne(eCollisionGroup::Platform))
			{
				gravitySpeed_ = 0.0f;
				bGround_ = true;
				bCanJump_ = true;
				bCanDash_ = true;
				playLandingSound();
				normalState_ << "Idle";
			}
			else
			{
				bGround_ = false;
				transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
				gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
			}
		}
		else
		{
			bGround_ = false;
			transform_->AddLocation(0.0f, gravitySpeed_ * _deltaTime);
			gravitySpeed_ -= GRAVITY_POWER * _deltaTime;
		}
	}

	// 총알 쏘는 방향
	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { -0.5f, 0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { 0.5f, 0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}
		}
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, 0.0f);
			bulletDirection_ = { -0.5f, -0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, 0.0f);
			bulletDirection_ = { 0.5f, -0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}
		}
	}
	else
	{
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_.z = 0.0f;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_.z = 0.0f;
		}
	}

	// 움직임
	if (GameEngineInput::GetInstance().IsKeyPress("Left"))
	{
		if (float4::BLACK != Map::GetColor(leftSideCollision_))
		{
			transform_->AddLocation(-MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = true;
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
	{
		if (float4::BLACK != Map::GetColor(rightSideCollision_))
		{
			transform_->AddLocation(MOVE_SPEED * _deltaTime, 0.0f);
		}
		bLeft_ = false;
	}
}

void Player::endParry(float _deltaTime)
{
	collider_->SetScale(70.f);
}

void Player::startEX(float _deltaTime)
{
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_ex_forward_ground_01.wav");
	bUpOrDown_ = false;

	if (GameEngineInput::GetInstance().IsKeyPress("Up"))
	{
		bUp_ = true;
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { -0.5f, 0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;

			if (bGround_)
			{
				renderer_->ChangeAnimation("SSGround_DiagonalUp");
			}
			else
			{
				renderer_->ChangeAnimation("SSAir_DiagonalUp");
			}
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_DIAGONALUP_OFFSET);
			bulletDirection_ = { 0.5f, 0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;

			if (bGround_)
			{
				renderer_->ChangeAnimation("SSGround_DiagonalUp");
			}
			else
			{
				renderer_->ChangeAnimation("SSAir_DiagonalUp");
			}

			bLeft_ = false;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_UP_OFFSET);
				bulletDirection_ = float4::UP;
				bulletRotation_.z = 90.f * GameEngineMath::DegreeToRadian;
			}

			if (bGround_)
			{
				renderer_->ChangeAnimation("SSGround_Up");
			}
			else
			{
				renderer_->ChangeAnimation("SSAir_Up");
			}
			bUpOrDown_ = true;
		}
	}
	else if (GameEngineInput::GetInstance().IsKeyPress("Down"))
	{
		bUp_ = false;
		if (GameEngineInput::GetInstance().IsKeyPress("Left"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, 0.0f);
			bulletDirection_ = { -0.5f, -0.5f };
			bulletRotation_.z = 45.f * GameEngineMath::DegreeToRadian;

			if (bGround_)
			{
				renderer_->ChangeAnimation("SSGround_DiagonalDown");
			}
			else
			{
				renderer_->ChangeAnimation("SSAir_DiagonalDown");
			}
			bLeft_ = true;
		}
		else if (GameEngineInput::GetInstance().IsKeyPress("Right"))
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, 0.0f);
			bulletDirection_ = { 0.5f, -0.5f };
			bulletRotation_.z = -45.f * GameEngineMath::DegreeToRadian;

			if (bGround_)
			{
				renderer_->ChangeAnimation("SSGround_DiagonalDown");
			}
			else
			{
				renderer_->ChangeAnimation("SSAir_DiagonalDown");
			}
			bLeft_ = false;
		}
		else
		{
			if (bLeft_)
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_LEFTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = 90 * GameEngineMath::DegreeToRadian;
			}
			else
			{
				bulletSpawnParentLocation_->SetLocation(BULLET_RIGHTUP_OFFSET, BULLET_DOWN_OFFSET);
				bulletDirection_ = float4::DOWN;
				bulletRotation_.z = -90.f * GameEngineMath::DegreeToRadian;
			}

			if (bGround_)
			{
				renderer_->ChangeAnimation("SSGround_Down");
			}
			else
			{
				renderer_->ChangeAnimation("SSAir_Down");
			}
			bUpOrDown_ = true;
		}
	}
	else
	{
		if (bGround_)
		{
			renderer_->ChangeAnimation("SSGround_Straight");
		}
		else
		{
			renderer_->ChangeAnimation("SSAir_Straight");
		}

		if (bLeft_)
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_LEFT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::LEFT;
			bulletRotation_ = float4::ZERO;
		}
		else
		{
			bulletSpawnParentLocation_->SetLocation(BULLET_RIGHT_OFFSET, BULLET_STRAIGHT_OFFSET);
			bulletDirection_ = float4::RIGHT;
			bulletRotation_ = float4::ZERO;
		}
	}
}

void Player::updateEX(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->CurFrame_ > 4)
	{
		if (normalState_.GetTime() < 10.f)
		{
			float x = 0.0f;
			if (bUpOrDown_ && bUp_ == false)
			{
				if (bLeft_)
				{
					x = -50.f;
				}
				else
				{
					x = 50.f;
				}
			}
			else if (bUpOrDown_ && bUp_ == true)
			{
				if (bLeft_)
				{
					x = 50.f;
				}
				else
				{
					x = -50.f;
				}
			}

			EXDust* dust = level_->CreateActor<EXDust>();
			dust->GetTransform()->SetLocation(transform_->GetWorldLocation() + float4(x, 60.f));
			dust->Initialize(bLeft_, bulletRotation_.z);

			PeashotEx* newShot = level_->CreateActor<PeashotEx>("PeashotEX");
			newShot->GetTransform()->SetLocation(bulletSpawnLocation_->GetWorldLocation());
			newShot->InitBullet(bLeft_, bulletDirection_, bulletRotation_);


			normalState_.SetTime(100.f);
		}

		if (!bUpOrDown_)
		{
			if (bLeft_)
			{
				if (float4::BLACK != Map::GetColor(rightSideCollision_))
				{
					transform_->AddLocation(150.f * _deltaTime, 0.0f);
				}

			}
			else
			{
				if (float4::BLACK != Map::GetColor(leftSideCollision_))
				{
					transform_->AddLocation(-150.f * _deltaTime, 0.0f);
				}
			}
		}
	}



	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		normalState_ << "Idle";
	}
}

void Player::startCinematicIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("Idle", true);
}

void Player::updateCinematicIdle(float _deltaTime)
{
	addGravity(_deltaTime);
}

void Player::startIntro(float _deltaTime)
{
	renderer_->ChangeAnimation("Intro");
}

void Player::updateIntro(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		normalState_ << "Idle";
		state_ << "NormalState";
	}
}

void Player::startScared(float _deltaTime)
{
	renderer_->ChangeAnimation("Scared", true);
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_intro_scared.wav");
}

void Player::updateScared(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "NormalState";
		return;
	}
}

void Player::startScaredWait(float _deltaTime)
{
	renderer_->ChangeAnimation("Scared", true);
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_intro_scared.wav");
}

void Player::updateScaredWait(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("Idle", true);
	}
}

void Player::startDevilPhaseOneEndFalling(float _deltaTime)
{
	renderer_->ChangeAnimation("Air");
}

void Player::updateDevilPhaseOneEndFalling(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	if (timeCounter_ < 2.0f)
	{
		transform_->AddLocation(0.0f, -1000.f * _deltaTime);
	}
	else
	{
		transform_->SetLocationY(-4000.f);
		state_ << "NormalState";
		normalState_ << "Idle";
	}
}
