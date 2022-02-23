#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineRenderingComponent.h"
#include "GameEngineCamera.h"

GameEngineLevel::GameEngineLevel()
	: mainCamera_(nullptr)
{

}

GameEngineLevel::~GameEngineLevel()
{
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		for (GameEngineActor* actor : pair.second)
		{
			if (nullptr != actor)
			{
				delete actor;
				actor = nullptr;
			}
		}
	}
}

void GameEngineLevel::ActorUpdate(float _deltaTime)
{
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		for (GameEngineActor* actor : pair.second)
		{
			if (actor->IsUpdate())
			{
				actor->transformUpdate();
				actor->updateComponent();
				actor->Update(_deltaTime);
			}
		}
	}
}

void GameEngineLevel::Render()
{
	GameEngineDevice::GetInst().RenderStart();

	float4x4 viewMatrix = mainCamera_->getViewMatrix();
	float4x4 projectionMatrix = mainCamera_->getProjectionMatrix();

	for (GameEngineRenderingComponent* obj : allRenderer_)
	{
		if (obj->IsUpdate())
		{
			obj->GetTransform()->GetTransformData().View = viewMatrix;
			obj->GetTransform()->GetTransformData().Projection = projectionMatrix;
			obj->Render();
		}
	}


	GameEngineDevice::GetInst().RenderEnd();
}

GameEngineCamera* GameEngineLevel::GetMainCameraActor()
{
	return mainCamera_;
}

GameEngineCameraComponent* GameEngineLevel::GetMainCameraComponent()
{
	return mainCamera_->GetCamera();
}

void GameEngineLevel::init()
{
	mainCamera_ = CreateActor<GameEngineCamera>("MainCamera");
}

void GameEngineLevel::pushRenderingComponent(GameEngineRenderingComponent* _renderingComponent)
{
	allRenderer_.push_back(_renderingComponent);
}
