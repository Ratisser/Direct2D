#include "PreCompile.h"
#include "DevilMap.h"

#include <GameEngine\GameEngineLevel.h>

DevilMap::DevilMap()
	: hallFrontLayer_(nullptr)
	, chairLayer_(nullptr)
{

}

DevilMap::~DevilMap()
{

}

void DevilMap::Start()
{
	Map::Start();

	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("BossDevil_Phase1PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);
	ImageRenderer->Off();

	// background
	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("BossDevil_Phase1Background.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 2.f);
	}

	// chair layer
	{
		chairLayer_ = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		chairLayer_->SetTexture("BossDevil_Phase1BackgroundChair.png", true);
		float4 textureSize = chairLayer_->GetCurrentTexture()->GetTextureSize();
		chairLayer_->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 1.5f);
	}

	// hall front layer
	{
		hallFrontLayer_ = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		hallFrontLayer_->SetTexture("HallFront.png", true);
		float4 textureSize = hallFrontLayer_->GetCurrentTexture()->GetTextureSize();
		hallFrontLayer_->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -1.f);
		hallFrontLayer_->Off();
	}

	// pot layer
	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("BossDevil_Phase1FrontLayerBackground.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -2.f);
	}


}

void DevilMap::Update(float _deltaTime)
{
}

void DevilMap::HallFrontLayerOn()
{
	hallFrontLayer_->On();
}

void DevilMap::HallFrontLayerOff()
{
	hallFrontLayer_->Off();
}

void DevilMap::ChangeCollisionPhaseTwo()
{
	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("BossDevil_Phase2PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);
	ImageRenderer->On();
	chairLayer_->Off();
}
