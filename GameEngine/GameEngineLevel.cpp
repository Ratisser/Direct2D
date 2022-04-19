#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineRenderer.h"
#include "GameEngineInput.h"
#include <GameEngine\GameEngineGUI.h>

GameEngineLevel::GameEngineLevel()
	: mainCamera_(nullptr)
	, uiCamera_(nullptr)
	, freeCamera_(nullptr)
	, mainCameraBackup_(nullptr)
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

void GameEngineLevel::LevelUpdateAfter(float _deltaTime)
{
	GameEngineInput* input = &GameEngineInput::GetInstance();
	if (input->IsKeyDown("O"))
	{
		SwitchFreeCamera();
	}

	if (mainCamera_ == freeCamera_)
	{
		const float CAMERA_MOVE_SPEED = 500.f;
		const float CAMERA_MOVE_SPEED_BOOST = 1500.f;


		float CameraMoveSpeed = CAMERA_MOVE_SPEED;
		if (input->IsKeyPress("LShift"))
		{
			CameraMoveSpeed = CAMERA_MOVE_SPEED_BOOST;
		}

		GameEngineCameraComponent* cam = freeCamera_->GetCameraComponent();
		if (input->IsKeyPress("W"))
		{
			cam->AddLocation(cam->GetForward() * CameraMoveSpeed * _deltaTime);
		}
		if (input->IsKeyPress("S"))
		{
			cam->AddLocation(cam->GetBackward() * CameraMoveSpeed * _deltaTime);
		}
		if (input->IsKeyPress("A"))
		{
			cam->AddLocation(cam->GetLeft() * CameraMoveSpeed * _deltaTime);
		}
		if (input->IsKeyPress("D"))
		{
			cam->AddLocation(cam->GetRight() * CameraMoveSpeed * _deltaTime);
		}
		if (input->IsKeyPress("Q"))
		{
			cam->AddLocation(cam->GetDown() * CameraMoveSpeed * _deltaTime);
		}
		if (input->IsKeyPress("E"))
		{
			cam->AddLocation(cam->GetUp() * CameraMoveSpeed * _deltaTime);
		}

		if (input->IsKeyPress("RClick"))
		{
			float4 mouseDelta = input->GetMouseDirection();
			cam->AddRotation(-mouseDelta.y * GameEngineMath::DegreeToRadian, mouseDelta.x * GameEngineMath::DegreeToRadian, 0.0f);
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

bool ZSort(GameEngineRenderer* _lhs, GameEngineRenderer* _rhs)
{
	return _lhs->GetWorldLocation().z > _rhs->GetWorldLocation().z;
}

void GameEngineLevel::Render()
{
	GameEngineDevice::GetInst().RenderStart();

	float4x4 viewMatrix = mainCamera_->getViewMatrix();
	float4x4 projectionMatrix = mainCamera_->getProjectionMatrix();

	allRenderer_.sort(ZSort);

	for (GameEngineRenderer* obj : allRenderer_)
	{
		if (obj->IsUpdate())
		{
			obj->GetTransformData().View = viewMatrix;
			obj->GetTransformData().Projection = projectionMatrix;
			obj->Render();
		}
	}


	GameEngineGUI::GetInst()->GUIRenderStart();
	GameEngineGUI::GetInst()->GUIRenderEnd();

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
	//				GameEngineDebug::MsgBoxError("Release actor Is Nullptr!!!!");
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

void GameEngineLevel::SwitchFreeCamera()
{
	if (mainCamera_ == mainCameraBackup_)
	{
		mainCamera_ = freeCamera_;
	}
	else
	{
		mainCamera_ = mainCameraBackup_;
	}
}

bool GameEngineLevel::IsFreeCamera()
{
	return mainCamera_ == freeCamera_;
}

GameEngineCamera* GameEngineLevel::GetMainCameraActor()
{
	return mainCamera_;
}

GameEngineCameraComponent* GameEngineLevel::GetMainCameraComponent()
{
	return mainCamera_->GetCameraComponent();
}

std::list<GameEngineCollision*>& GameEngineLevel::GetCollisionGroup(int _group)
{
	return allCollisions_[_group];
}

void GameEngineLevel::init()
{
	mainCamera_ = CreateActor<GameEngineCamera>("MainCamera");
	mainCameraBackup_ = mainCamera_;
	uiCamera_ = nullptr;
	freeCamera_ = CreateActor<GameEngineCamera>("FreeCamera");
	freeCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Perspective);

	GameEngineInput::GetInstance().CreateKey("W", 'W');
	GameEngineInput::GetInstance().CreateKey("A", 'A');
	GameEngineInput::GetInstance().CreateKey("S", 'S');
	GameEngineInput::GetInstance().CreateKey("D", 'D');
	GameEngineInput::GetInstance().CreateKey("Q", 'Q');
	GameEngineInput::GetInstance().CreateKey("E", 'E');
	GameEngineInput::GetInstance().CreateKey("O", 'O');

	GameEngineInput::GetInstance().CreateKey("LShift", VK_LSHIFT);
	GameEngineInput::GetInstance().CreateKey("RClick", VK_RBUTTON);
}

void GameEngineLevel::pushRenderer(GameEngineRenderer* _renderingComponent)
{
	allRenderer_.push_back(_renderingComponent);
}

void GameEngineLevel::pushCollision(int _group, GameEngineCollision* _collision)
{
	allCollisions_[_group].push_back(_collision);
}

void GameEngineLevel::popCollision(int _group, GameEngineCollision* _collision)
{
	allCollisions_[_group].remove(_collision);
}

void GameEngineLevel::popRenderer(GameEngineRenderer* _renderingComponent)
{
	allRenderer_.remove(_renderingComponent);
}

void GameEngineLevel::levelChangeStartActorEvent()
{
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		std::list<GameEngineActor*>& actors = pair.second;

		for (GameEngineActor* actor : actors)
		{
			actor->levelChangeStartEvent();
		}
	}
}

void GameEngineLevel::levelChangeEndActorEvent()
{
	for (std::pair<int, std::list<GameEngineActor*>> pair : allActors_)
	{
		std::list<GameEngineActor*>& actors = pair.second;

		for (GameEngineActor* actor : actors)
		{
			actor->levelChangeEndEvent();
		}
	}
}

