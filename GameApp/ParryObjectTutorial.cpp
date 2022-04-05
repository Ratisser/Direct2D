#include "PreCompile.h"
#include "ParryObjectTutorial.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>

ParryObjectTutorial::ParryObjectTutorial()
	: parryOff_(nullptr)
	, parryOn_(nullptr)
	, coolTime_(0.0f)
{

}

ParryObjectTutorial::~ParryObjectTutorial()
{

}

void ParryObjectTutorial::Start()
{
	ParryObjectBase::Start();

	parryOn_ = CreateTransformComponent<GameEngineImageRenderer>();
	parryOn_->SetTexture("ParryObjectOn.png", true);

	parryOff_ = CreateTransformComponent<GameEngineImageRenderer>();
	parryOff_->SetTexture("ParryObjectOff.png", true);

	collision_->SetParent(parryOff_);

	bParryable_ = false;
}

void ParryObjectTutorial::Update(float _deltaTime)
{
	ParryObjectBase::Update(_deltaTime);

	if (bParryable_)
	{
		parryOn_->On();
		parryOff_->Off();
		coolTime_ = 3.0f;
	}
	else
	{
		parryOn_->Off();
		parryOff_->On();
	}

	if (!bParryable_)
	{
		coolTime_ -=_deltaTime;
		if (coolTime_ < 0.0f)
		{
			bParryable_ = true;
		}
	}
}
