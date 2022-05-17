#include "PreCompile.h"
#include "Demon.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineGUI.h>

#include "GameEngineLevelControlWindow.h"

Demon::Demon()
	: renderer_(nullptr)
	, bodyCollision_(nullptr)
	, bLeft_(false)
	, timeCounter_(0.0f)
	, hitEffectTime_(0.0f)
{
}

Demon::~Demon()
{

}

void Demon::Start()
{
	SetHP(DEMON_HP);

	state_.CreateState("Run", std::bind(&Demon::startRun, this, std::placeholders::_1), std::bind(&Demon::updateRun, this, std::placeholders::_1));
	state_.CreateState("Attack", std::bind(&Demon::startAttack, this, std::placeholders::_1), std::bind(&Demon::updateAttack, this, std::placeholders::_1));
	state_.CreateState("Die", std::bind(&Demon::startDie, this, std::placeholders::_1), std::bind(&Demon::updateDie, this, std::placeholders::_1));
	state_.CreateState("End", std::bind(&Demon::startEnd, this, std::placeholders::_1), std::bind(&Demon::updateEnd, this, std::placeholders::_1));
	state_.ChangeState("Run");

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Explosion", 0.034f, false);
	renderer_->CreateAnimationFolder("DemonAttack");
	renderer_->CreateAnimationFolder("DemonJump", 0.034f, false);
	renderer_->ChangeAnimation("DemonAttack");

	bodyCollision_ = CreateTransformComponent<GameEngineCollision>();
	bodyCollision_->SetCollisionType(eCollisionType::Rect);
	bodyCollision_->SetCollisionGroup(eCollisionGroup::Monster);
	bodyCollision_->SetLocationY(96.f);
	bodyCollision_->SetScale(96.f);
	bodyCollision_->Off();
}

void Demon::Update(float _deltaTime)
{
	state_.Update(_deltaTime);

	if (hitEffectTime_ > 0.0f)
	{
		hitEffectTime_ -= _deltaTime;
	}
	else
	{
		renderer_->SetAddColor(float4::ZERO);
	}

	if (bLeft_)
	{
		renderer_->SetFlip(false, false);
	}
	else
	{
		renderer_->SetFlip(true, false);
	}
}

void Demon::startRun(float _deltaTime)
{
	transform_->SetScale(0.8f);
	renderer_->ChangeAnimation("DemonAttack", true);
	timeCounter_ = 0.0f;

	GameEngineRandom random;
	bLeft_ = static_cast<bool>(random.RandomInt(0, 1));

	int soundNumber = random.RandomInt(1, 2);
	std::string soundName = "sfx_devil_imp_spawn_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void Demon::updateRun(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (bLeft_)
	{
		transform_->SetLocation(GameEngineMath::Lerp(START_LOCATION, LEFT_RUN_END, timeCounter_, RUN_TIME));
	}
	else
	{
		transform_->SetLocation(GameEngineMath::Lerp(START_LOCATION, RIGHT_RUN_END, timeCounter_, RUN_TIME));
	}

	if (timeCounter_ > RUN_TIME)
	{
		state_ << "Attack";
		return;
	}

	//float4 mousePos = GameEngineInput::GetInstance().GetMousePosition();
	//GameEngineActor* camera = level_->GetMainCameraActor();
	//float4 cameraPos = float4::ZERO;

	//if (nullptr != camera)
	//{
	//	cameraPos = camera->GetTransform()->GetWorldLocation();
	//}

	//transform_->SetLocation(mousePos + cameraPos);

	//GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	//if (nullptr != controlWindow)
	//{
	//	controlWindow->AddText("blah blah");
	//}
}

void Demon::startAttack(float _deltaTime)
{
	transform_->SetScale(1.0f);
	bodyCollision_->On();

	// 갔던 곳에서 다시 오기 때문에 반대로 뒤집어준다.
	bLeft_ = !bLeft_;

	bodyCollision_->SetLocationX(40.f * (-1 * static_cast<int>(bLeft_)));

	timeCounter_ = 0.0f;
}

void Demon::updateAttack(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (bLeft_)
	{
		transform_->SetLocation(GameEngineMath::Lerp(RIGHT_ATTACK_START_LOCATION, LEFT_ATTACK_START_LOCATION, timeCounter_, ATTACK_TIME));
	}
	else
	{
		transform_->SetLocation(GameEngineMath::Lerp(LEFT_ATTACK_START_LOCATION, RIGHT_ATTACK_START_LOCATION, timeCounter_, ATTACK_TIME));
	}

	if (timeCounter_ > ATTACK_TIME)
	{
		state_ << "End";
		return;
	}
}

void Demon::startDie(float _deltaTime)
{
	transform_->SetScale(0.5f);
	transform_->AddLocation(0.0f, 50.f, 0.0f);
	renderer_->ChangeAnimation("Explosion", true);
	renderer_->SetPivot(eImagePivot::CENTER);
	bodyCollision_->Off();

	GameEngineRandom random;
	int soundNumber = random.RandomInt(1, 4);
	std::string soundName = "sfx_devil_imp_death_0" + std::to_string(soundNumber) + ".wav";
	GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}

void Demon::updateDie(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "End";
		return;
	}
}

void Demon::startEnd(float _deltaTime)
{
}

void Demon::updateEnd(float _deltaTime)
{
	Release();
}

void Demon::OnHit()
{
	renderer_->SetAddColor({ 0.1f, 0.2f, 0.3f });
	hitEffectTime_ = HIT_EFFECT_TIME;

	if (hp_ <= 0)
	{
		state_ << "Die";
		return;
	}
}
