#include "PreCompile.h"
#include "GameEngineTransformComponent.h"
#include "GameEngineTransform.h"

GameEngineTransformComponent::GameEngineTransformComponent()
	: transform_(new GameEngineTransform)
{
}

GameEngineTransformComponent::~GameEngineTransformComponent()
{
	delete transform_;
}