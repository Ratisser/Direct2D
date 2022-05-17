#include "PreCompile.h"
#include "Knockout.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

Knockout::Knockout()
	: renderer_(nullptr)
{

}

Knockout::~Knockout()
{

}

void Knockout::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	renderer_->CreateAnimationFolder("Knockout", 0.034f, false, false);
	renderer_->SetScale(1280.f, 720.f);
	renderer_->SetLocationZ(-4.0f);

	renderer_->ChangeAnimation("Knockout");
	
	SetBulletTimeEffect(false);
}

void Knockout::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->CurFrame_ > 10 && renderer_->GetCurrentAnimation()->CurFrame_ < 20)
	{
		renderer_->GetCurrentAnimation()->InterTime_ = 0.1f;
	}
	else
	{
		renderer_->GetCurrentAnimation()->InterTime_ = 0.034f;
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
