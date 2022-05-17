#include "PreCompile.h"
#include "Explosion.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

Explosion::Explosion()
	: renderer_(nullptr)
{

}

Explosion::~Explosion()
{

}

void Explosion::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("Explosion", 0.034f, false);
	renderer_->SetLocationZ(-0.5f);

	renderer_->ChangeAnimation("Explosion");
}

void Explosion::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
