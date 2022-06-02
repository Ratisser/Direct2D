#include "PreCompile.h"
#include "DashDust.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

DashDust::DashDust()
	: renderer_(nullptr)
	, childTransform_(nullptr)
{

}

DashDust::~DashDust()
{

}

void DashDust::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationY(-25.f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("DashDust", 0.0416f, false);
	renderer_->SetLocationZ(0.01f);

	renderer_->ChangeAnimation("DashDust");
}

void DashDust::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
