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

void GameEngineLevel::Release(float _deltaTime)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : allActors_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		for (GameEngineActor* Actor : Actors)
		{
			Actor->ReleaseUpdate(_deltaTime);
		}
	}

	//// 콜리전 삭제
	//{
	//	std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapBeginIter = CollisionList_.begin();
	//	std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapEndIter = CollisionList_.end();


	//	for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
	//	{
	//		std::list<GameEngineCollision*>& Collisions = RenderMapBeginIter->second;

	//		std::list<GameEngineCollision*>::iterator beginIter = Collisions.begin();
	//		std::list<GameEngineCollision*>::iterator endIter = Collisions.end();

	//		for (; beginIter != endIter; )
	//		{
	//			GameEngineCollision* ReleaseCollision = *beginIter;

	//			if (nullptr == ReleaseCollision)
	//			{
	//				GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
	//			}

	//			if (true == ReleaseCollision->IsDeath())
	//			{
	//				beginIter = Collisions.erase(beginIter);

	//				continue;
	//			}

	//			++beginIter;

	//		}
	//	}
	//}


	std::map<int, std::list<GameEngineActor*>>::iterator actorMapBeginIter = allActors_.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator actorMapEndIter = allActors_.end();

	for (; actorMapBeginIter != actorMapEndIter; ++actorMapBeginIter)
	{
		std::list<GameEngineActor*>& Actors = actorMapBeginIter->second;

		std::list<GameEngineActor*>::iterator beginIter = Actors.begin();
		std::list<GameEngineActor*>::iterator endIter = Actors.end();

		for (; beginIter != endIter; )
		{
			GameEngineActor* ReleaseActor = *beginIter;

			if (nullptr == ReleaseActor)
			{
				GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
			}

			ReleaseActor->ReleaseComponent();

			if (true == ReleaseActor->IsDeath())
			{
				delete* beginIter;
				*beginIter = nullptr;

				beginIter = Actors.erase(beginIter);

				continue;
			}

			++beginIter;
		}
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

void GameEngineLevel::pushRenderer(GameEngineRenderer* _renderingComponent)
{
	allRenderer_.insert(_renderingComponent);
}

void GameEngineLevel::popRenderer(GameEngineRenderer* _renderingComponent)
{
	allRenderer_.erase(_renderingComponent);
}
