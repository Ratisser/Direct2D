#include "PreCompile.h"
#include "Flower.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineInput.h>

Flower::Flower()
	: renderer_(nullptr)
	, bodyTransform_(nullptr)
	, collision_(nullptr)
	, nextState_(0)
{

}

Flower::~Flower()
{

}

void Flower::Start()
{
	SetHP(HP);
	initInput();
	initTransform();
	initRendererAndAnimation();
	initCollision();
	initState();
}

void Flower::Update(float _deltaTime)
{
	GameEngineInput& input = GameEngineInput::GetInstance();
	for (int i = 1; i < 5; i++)
	{
		if (input.IsKeyDown(std::to_string(i)))
		{
			nextState_ = i;
		}
	}

	state_.Update(_deltaTime);
}

void Flower::OnHit()
{
}

void Flower::initInput()
{
}

void Flower::initTransform()
{
	bodyTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
}

void Flower::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(bodyTransform_);
	renderer_->CreateAnimationFolder("FlowerIntro", 1.0f, false);
	renderer_->CreateAnimationFolder("FlowerIdle", 0.0416f);
	renderer_->CreateAnimationFolder("FaceAttackHighBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("FaceAttackHighIdle", 0.067f);
	renderer_->CreateAnimationFolder("FaceAttackHighEnd", 0.067f, false);
	renderer_->CreateAnimationFolder("FaceAttackLowBegin", 0.0416f, false);
	renderer_->CreateAnimationFolder("FaceAttackLowIdle", 0.067f);
	renderer_->CreateAnimationFolder("FaceAttackLowEnd", 0.067f, false);

	renderer_->SetPivot(eImagePivot::BOTTOM_RIGHT);
	renderer_->ChangeAnimation("FlowerIntro");
	
}

void Flower::initCollision()
{
	collision_ = CreateTransformComponent<GameEngineCollision>(bodyTransform_);
	collision_->SetScale(400.f, 600.f);
	collision_->SetLocation(-290.f, 300.f);
	collision_->SetCollisionGroup(eCollisionGroup::Monster);
	collision_->SetCollisionType(eCollisionType::Rect);
}

void Flower::initState()
{
	state_.CreateState(MakeState(Flower, Intro));
	state_.CreateState(MakeState(Flower, Idle));
	state_.CreateState(MakeState(Flower, FaceAttackHighBegin));
	state_.CreateState(MakeState(Flower, FaceAttackHighIdle));
	state_.CreateState(MakeState(Flower, FaceAttackHighEnd));
	state_.CreateState(MakeState(Flower, FaceAttackLowBegin));
	state_.CreateState(MakeState(Flower, FaceAttackLowIdle));
	state_.CreateState(MakeState(Flower, FaceAttackLowEnd));

	state_ << "Intro";
}

void Flower::startIntro(float _deltaTime)
{
	
}

void Flower::updateIntro(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_intro_yell.wav");
		renderer_->GetCurrentAnimation()->InterTime_ = 0.0416f;
		state_.SetTime(-10.f);
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "Idle";
	}
}

void Flower::startIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FlowerIdle");
}

void Flower::updateIdle(float _deltaTime)
{
	float time = state_.GetTime();
	if (time > ACTION_DELAY)
	{
		if (hp_ <= 100)
		{

			state_ << "EnterPhaseFour";
			return;
		}

		GameEngineRandom random;

		eAttackState as = static_cast<eAttackState>(random.RandomInt(0, static_cast<int>(eAttackState::MAX_COUNT) - 1));

		if (0 != nextState_)
		{
			as = static_cast<eAttackState>(nextState_);
		}

		switch (as)
		{
		case Flower::FACE_ATTACK_HIGH:
			state_ << "FaceAttackHighBegin";
			break;
		case Flower::FACE_ATTACK_LOW:
			state_ << "FaceAttackLowBegin";
			break;
		case Flower::GATLING:
			break;
		case Flower::MAX_COUNT:
			break;
		default:
			break;
		}

		nextState_ = 0;
	}
}

void Flower::startFaceAttackHighBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackHighBegin");

	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_start.wav");
}

void Flower::updateFaceAttackHighBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "FaceAttackHighIdle";
	}
}

void Flower::startFaceAttackHighIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackHighIdle");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_hold_loop.wav");
}

void Flower::updateFaceAttackHighIdle(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		state_ << "FaceAttackHighEnd";
	}
}

void Flower::startFaceAttackHighEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackHighEnd");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_end.wav");
}

void Flower::updateFaceAttackHighEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FlowerIdle");
		renderer_->GetCurrentAnimation()->CurFrame_ = 18;
		state_ << "Idle";
	}
}

void Flower::startFaceAttackLowBegin(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackLowBegin");
	bodyTransform_->AddLocation(0.0f, -110.f);
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_start.wav");
}

void Flower::updateFaceAttackLowBegin(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		state_ << "FaceAttackLowIdle";
	}
}

void Flower::startFaceAttackLowIdle(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackLowIdle");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_hold_loop.wav");
}

void Flower::updateFaceAttackLowIdle(float _deltaTime)
{
	if (state_.GetTime() > 1.0f)
	{
		state_ << "FaceAttackLowEnd";
	}
}

void Flower::startFaceAttackLowEnd(float _deltaTime)
{
	renderer_->ChangeAnimation("FaceAttackLowEnd");
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_flower_top_laser_attack_end.wav");
}

void Flower::updateFaceAttackLowEnd(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		renderer_->ChangeAnimation("FlowerIdle");
		bodyTransform_->AddLocation(0.0f, 110.f);
		state_ << "Idle";
	}
}
