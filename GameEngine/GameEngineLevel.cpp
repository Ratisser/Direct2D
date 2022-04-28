#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineRenderer.h"
#include "GameEngineInput.h"
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineTexture.h"
#include "GameEngineTextureManager.h"
#include <GameEngine\GameEngineWindow.h>
#include <GameEngine\GameEngineRenderTargetManager.h>
#include "GameEngineShaderResHelper.h"


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

struct BlurData
{
	int FilterStartX; // -2
	int FilterStartY; // -2
	int FilterEndX; // 2
	int FilterEndY; // 2
	int FilterCount;
	float FilterSum;
	float ImagePixelUVX;
	float ImagePixelUVY;
	float FilterPixelX;
	float FilterPixelY;
	float Temp0;
	float Temp1;
};


void GameEngineLevel::Render()
{
	GameEngineDevice::GetInst().RenderStart();

	float4x4 viewMatrix = mainCamera_->getViewMatrix();
	float4x4 projectionMatrix = mainCamera_->getProjectionMatrix();

	allRenderer_.sort(ZSort);

	//GameEngineRenderTarget* mainRenderTarget = GameEngineRenderTargetManager::GetInst().Find("MainRenderTarget");
	//mainRenderTarget->Clear();
	//mainRenderTarget->Setting();

	for (GameEngineRenderer* obj : allRenderer_)
	{
		if (obj->IsUpdate())
		{
			obj->GetTransformData().View = viewMatrix;
			obj->GetTransformData().Projection = projectionMatrix;
			obj->Render();
		}
	}

	for (size_t i = 0; i < 1; i++)
	{
		GameEngineRenderTarget* backbufferTarget = GameEngineDevice::GetBackbufferTarget();
		GameEngineTexture* texture = backbufferTarget->GetTexture(0);

		float4 ScreenSize = GameEngineWindow::GetInst().GetSize();

		//D3D11_TEXTURE2D_DESC desc;
		//texture->GetTexture()->GetDesc(&desc);
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = ScreenSize.ix();
		descDepth.Height = ScreenSize.iy();
		descDepth.MipLevels = 0;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Texture2D* tempTexture;
		if (S_OK != GameEngineDevice::GetDevice()->CreateTexture2D(&descDepth, NULL, &tempTexture))
		{
			GameEngineDebug::MsgBoxError("create texture Error");
			return;
		}

		GameEngineTexture* resultTexture = new GameEngineTexture();
		resultTexture->Create(tempTexture);

		ID3D11RenderTargetView* tempTarget = nullptr;
		GameEngineDevice::GetDevice()->CreateRenderTargetView(tempTexture, nullptr, &tempTarget);
		if (tempTarget == nullptr)
		{
			GameEngineDebug::AssertFalse();
			return;
		}
		GameEngineDevice::GetContext()->ClearRenderTargetView(tempTarget, float4::ONE.Arr1D);
		GameEngineDevice::GetContext()->OMSetRenderTargets(1, &tempTarget, nullptr);

		{

			GameEngineRenderer* renderer = new GameEngineRenderer();
			renderer->SetRenderingPipeline("Blur");
			renderer->ShaderHelper_.SettingTexture("Target", texture);
			renderer->ShaderHelper_.SettingTexture("Filter", "BlurFilter.png");


			GameEngineTexture* Filter = GameEngineTextureManager::GetInst().Find("BlurFilter.png");

			if (nullptr == Filter)
			{
				return;
			}

			BlurData Data;

			Data.FilterPixelX = 1.0f / Filter->GetTextureSize().x;
			Data.FilterPixelY = 1.0f / Filter->GetTextureSize().y;
			Data.FilterEndX = Filter->GetTextureSize().ix() / 2;
			Data.FilterEndY = Filter->GetTextureSize().iy() / 2;
			Data.FilterStartX = -Data.FilterEndX;
			Data.FilterStartY = -Data.FilterEndY;

			Data.ImagePixelUVX = 1.0f / texture->GetTextureSize().x;
			Data.ImagePixelUVY = 1.0f / texture->GetTextureSize().y;

			Data.FilterSum = 0.0f;
			for (int y = 0; y < Filter->GetTextureSize().iy(); y++)
			{
				for (int x = 0; x < Filter->GetTextureSize().ix(); x++)
				{
					Data.FilterSum += Filter->GetPixel(x, y).x;
				}
			}

			Data.FilterCount = Filter->GetTextureSize().ix() * Filter->GetTextureSize().iy();

			renderer->ShaderHelper_.SettingConstantBufferLink("BlurData", Data);

			renderer->ShaderHelper_.Setting();

			renderer->Render();
			delete renderer;
		}

		backbufferTarget->Clear();
		GameEngineDevice::GetContext()->ClearDepthStencilView(GameEngineDevice::GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		backbufferTarget->Setting();

		GameEngineRenderer* renderer2 = new GameEngineRenderer();
		renderer2->SetRenderingPipeline("TargetMerge");
		renderer2->ShaderHelper_.SettingTexture("Tex", resultTexture);
		renderer2->ShaderHelper_.Setting();

		renderer2->Render();
		delete renderer2;

		tempTarget->Release();

		delete resultTexture;
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

	static bool a = false;

	if (!a)
	{
		float4 ScreenSize = GameEngineWindow::GetInst().GetSize();

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = ScreenSize.ix();
		descDepth.Height = ScreenSize.iy();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		ID3D11Texture2D* mainTexture = nullptr;
		if (S_OK != GameEngineDevice::GetDevice()->CreateTexture2D(&descDepth, NULL, &mainTexture))
		{
			GameEngineDebug::MsgBoxError("create texture Error");
			return;
		}

		GameEngineTexture *texture = GameEngineTextureManager::GetInst().Create("MainTexture", mainTexture);

		GameEngineRenderTargetManager::GetInst().Create("MainRenderTarget", "MainTexture", float4::BLACK);


		a = true;
	}


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

