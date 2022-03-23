#include "PreCompile.h"
#include "TutorialMap.h"

#include <GameEngine\GameEngineLevel.h>

TutorialMap::TutorialMap()
{

}

TutorialMap::~TutorialMap()
{

}

void TutorialMap::Start()
{
	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("TutorialScene_Background_PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);

	{
		GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
		r->SetTexture("tutorial_room_back_layer_0001.png");
		r->SetScale(1280.f, 720.f);
		r->SetLocationZ(4.0f);
	}

	GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	r->SetTexture("tutorial_room_front_layer_0001.png", false);
	r->SetScale(1280.f, 720.f);
	r->SetLocationZ(2.0f);
}

void TutorialMap::Update(float _deltaTime)
{
}
