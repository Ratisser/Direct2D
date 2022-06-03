#include "PreCompile.h"
#include "Dust.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

Dust::Dust()
	: renderer_(nullptr)
	, childTransform_(nullptr)
{

}

Dust::~Dust()
{

}

void Dust::Start()
{
	childTransform_ = CreateTransformComponent<GameEngineTransformComponent>();
	childTransform_->SetLocationY(-10.f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(childTransform_);
	renderer_->CreateAnimationFolder("DustA", 0.0416f, false);
	renderer_->SetLocationZ(-0.01f);
	//renderer_->SetPivot(eImagePivot::CENTER);

	renderer_->ChangeAnimation("DustA");
}


void Dust::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
