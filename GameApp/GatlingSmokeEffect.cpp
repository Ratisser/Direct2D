#include "PreCompile.h"
#include "GatlingSmokeEffect.h"

#include <GameEngine\GameEngineImageRenderer.h>

GatlingSmokeEffect::GatlingSmokeEffect()
	: renderer_(nullptr)
{

}

GatlingSmokeEffect::~GatlingSmokeEffect()
{

}

void GatlingSmokeEffect::Start()
{
	transform_->SetLocationZ(-0.1f);

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("GatlingSmokeEffect", 0.0416f, false);
	renderer_->ChangeAnimation("GatlingSmokeEffect");
}

void GatlingSmokeEffect::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
