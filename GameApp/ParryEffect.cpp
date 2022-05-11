#include "PreCompile.h"
#include "ParryEffect.h"

#include <GameEngine\GameEngineImageRenderer.h>

ParryEffect::ParryEffect() 
	: parryEffectRenderer_(nullptr)
{

}

ParryEffect::~ParryEffect()
{

}

void ParryEffect::Start()
{
	parryEffectRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	parryEffectRenderer_->CreateAnimationFolder("ParryEffect", 0.034f, false);
	parryEffectRenderer_->ChangeAnimation("ParryEffect");
	parryEffectRenderer_->SetPivot(eImagePivot::CENTER);
}

void ParryEffect::Update(float _deltaTime)
{
	if (parryEffectRenderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}

