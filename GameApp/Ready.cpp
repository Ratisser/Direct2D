#include "PreCompile.h"
#include "Ready.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

Ready::Ready()
	: renderer_(nullptr)
	, timeCounter_(0.0f)
{

}

Ready::~Ready()
{

}

void Ready::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	renderer_->CreateAnimationFolder("ReadyWALLOP!", 0.0416f, false, false);
	renderer_->SetScale(1280.f, 720.f);
	renderer_->SetLocationZ(-6.0f);

	renderer_->ChangeAnimation("ReadyWALLOP!");
	renderer_->Off();

	SetBulletTimeEffect(false);
}

void Ready::Update(float _deltaTime)
{
	timeCounter_ += _deltaTime;
	if (timeCounter_ > 0.5f)
	{
		renderer_->On();
		timeCounter_ = -100.f;
	}

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
