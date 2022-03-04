#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineRenderer.h"
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
				actor->Update(_deltaTime);
				actor->updateComponent(_deltaTime);
			}
		}
	}

	// 업데이트를 전부 수행한 후 트랜스폼 업데이트를 수행한다.
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		for (GameEngineActor* actor : pair.second)
		{
			if (actor->IsUpdate())
			{
				actor->transformUpdate();
			}
		}
	}
}

void GameEngineLevel::Render()
{
	GameEngineDevice::GetInst().RenderStart();

	float4x4 viewMatrix = mainCamera_->getViewMatrix();
	float4x4 projectionMatrix = mainCamera_->getProjectionMatrix();

	for (GameEngineRenderer* obj : allRenderer_)
	{
		if (obj->IsUpdate())
		{
			obj->GetTransformData().View = viewMatrix;
			obj->GetTransformData().Projection = projectionMatrix;
			obj->Render();
		}
	}


	GameEngineDevice::GetInst().RenderEnd();
}

void GameEngineLevel::ActorReleaseUpdate()
{
	std::map<int, std::list<GameEngineActor*>>::iterator mapStartIter = allActors_.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator mapEndIter = allActors_.end();

	for (; mapStartIter != mapEndIter; ++mapStartIter)
	{

	}
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

void GameEngineLevel::pushRenderingComponent(GameEngineRenderer* _renderingComponent)
{
	allRenderer_.push_back(_renderingComponent);
}
