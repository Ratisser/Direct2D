#include "PreCompile.h"
#include "Ready.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

Ready::Ready()
	: renderer_(nullptr)
{

}

Ready::~Ready()
{

}

void Ready::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	renderer_->CreateAnimationFolder("ReadyWALLOP!", 0.034f, false, false);
	renderer_->SetScale(1280.f, 720.f);
	renderer_->SetLocationZ(-4.0f);

	renderer_->ChangeAnimation("ReadyWALLOP!");

	SetBulletTimeEffect(false);
}

void Ready::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
