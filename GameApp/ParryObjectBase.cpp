#include "PreCompile.h"
#include "ParryObjectBase.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineRenderer.h>
#include "eCollisionGroup.h"

ParryObjectBase::ParryObjectBase()
	: collision_(nullptr)
{

}

ParryObjectBase::~ParryObjectBase()
{

}

void ParryObjectBase::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetCollisionGroup(eCollisionGroup::ParryObject);

#ifdef _DEBUG
	GameEngineRenderer* newRenderer;
	newRenderer = CreateTransformComponent<GameEngineRenderer>(collision_);
	newRenderer->SetRenderingPipeline("DebugRect");
	newRenderer->SetLocationZ(-1.0f);
#endif // _DEBUG

}

void ParryObjectBase::Update(float _deltaTime)
{
}
