#include "PreCompile.h"
#include "TutorialMap.h"

#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineInput.h>

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineCore.h>

#include "eCollisionGroup.h"
#include <GameApp\FadeOut.h>

TutorialMap::TutorialMap()
	: exitDoor_(nullptr)
	, nextLevel_(false)
{

}

TutorialMap::~TutorialMap()
{

}

void TutorialMap::Start()
{
	Map::Start();

	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("TutorialScene_Background_PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);
	ImageRenderer->Off();

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("tutorial_object.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 2.f);
	}
	{
		GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
		r->SetTexture("tutorial_room_back_layer_0001.png");
		r->SetScale(1280.f, 720.f);
		r->SetLocationZ(20.f);
	}

	{
		GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
		r->SetTexture("tutorial_room_front_layer_0001.png", false);
		r->SetScale(1280.f, 720.f);
		r->SetLocationZ(0.1f);
	}

	exitDoor_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	exitDoor_->SetCollisionType(eCollisionType::Rect);
	exitDoor_->SetCollisionGroup(eCollisionGroup::Location);
	exitDoor_->SetScale(180.f, 300.f, 1.0f);
	exitDoor_->SetLocation(5760.f, -558.f, 0.0f);

//#ifdef _DEBUG
//	GameEngineRenderer* debugRect = CreateTransformComponent<GameEngineRenderer>(exitDoor_);
//	debugRect->SetRenderingPipeline("DebugRect");
//#endif // _DEBUG

}

void TutorialMap::Update(float _deltaTime)
{
	if (nextLevel_)
	{
		static float time = 0.0f;
		time += _deltaTime;
		if (time > 0.55f)
		{
			GameEngineCore::ChangeLevel("WorldLevel");
		}
	}

	if (nullptr != exitDoor_->IsCollideOne(eCollisionGroup::Player))
	{
		if (nextLevel_ == false && GameEngineInput::GetInstance().IsKeyDown("Z"))
		{
			nextLevel_ = true;
			level_->CreateActor<FadeOut>();
		}
	}

	if (GameEngineInput::GetInstance().IsKeyDown("P"))
	{
		GameEngineCore::ChangeLevel("WorldLevel");
		if (ImageRenderer->IsUpdate())
		{
			ImageRenderer->Off();
		}
		else
		{
			ImageRenderer->On();
		}
	}
}
