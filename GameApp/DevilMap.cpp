#include "PreCompile.h"
#include "DevilMap.h"

#include <GameEngine\GameEngineLevel.h>

DevilMap::DevilMap()
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

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("BossDevil_Phase1Background.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 2.f);
	}

	//{
	//	GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	//	mapObjects->SetTexture("BossDevil_Phase1Background2.png", true);
	//	float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
	//	mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 1.f);
	//}

	{
		hallFrontLayer_ = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		hallFrontLayer_->SetTexture("HallFront.png", true);
		float4 textureSize = hallFrontLayer_->GetCurrentTexture()->GetTextureSize();
		hallFrontLayer_->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -1.f);
		hallFrontLayer_->Off();
	}

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
