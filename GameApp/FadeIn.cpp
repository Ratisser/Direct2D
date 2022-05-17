#include "PreCompile.h"
#include "FadeIn.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

FadeIn::FadeIn()
	: renderer_(nullptr)
{

}

FadeIn::~FadeIn()
{

}

void FadeIn::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	renderer_->CreateAnimationFolder("Iris", 0.034f, false, false);
	renderer_->SetScale(1280.f, 720.f);
	renderer_->SetLocationZ(-4.0f);

	renderer_->ChangeAnimation("Iris");
	
	SetBulletTimeEffect(false);
}

void FadeIn::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
