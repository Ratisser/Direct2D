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
	renderer_->CreateAnimationFolder("GatlingSmokeEffect", 0.1f, false);
	renderer_->ChangeAnimation("GatlingSmokeEffect");
}

void GatlingSmokeEffect::Update(float _deltaTime)
{
	//transform_->AddLocation(0.0f, 200.f * _deltaTime);

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
