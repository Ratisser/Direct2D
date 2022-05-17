#include "PreCompile.h"
#include "Hourglass.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

Hourglass::Hourglass()
	: renderer_(nullptr)
{

}

Hourglass::~Hourglass()
{

}

void Hourglass::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Hourglass");
	renderer_->ChangeAnimation("Hourglass");

	transform_->SetLocation(500.0f, -300.f, 10.f);

	{
		GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
		float4 color = float4::ONE;
		color.a = 1.0f;
		r->SetColor(color);
		r->SetLocationZ(-10.0f);
		r->CreateAnimationFolder("OldFilmEffect", 0.034f, true, false);
		r->SetScale(1280.f, 720.f);
		r->ChangeAnimation("OldFilmEffect");
	}
}

void Hourglass::Update(float _deltaTime)
{
}
