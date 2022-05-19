#include "PreCompile.h"
#include "Flower.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

Flower::Flower()
	: renderer_(nullptr)
	, bodyTransform_(nullptr)
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

	renderer_->SetPivot(eImagePivot::BOTTOM_RIGHT);
	renderer_->ChangeAnimation("FlowerIntro");
	
}

void Flower::initCollision()
{
}

void Flower::initState()
{
	state_.CreateState(MakeState(Flower, Intro));
	state_.CreateState(MakeState(Flower, Idle));

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
		renderer_->GetCurrentAnimation()->InterTime_ = 0.0333f;
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
}
