#include "PreCompile.h"
#include "WorldMap.h"
#include <GameEngine\GameEngineInput.h>

#include "WorldMapPlayer.h"

WorldMap::WorldMap()
{

}

WorldMap::~WorldMap()
{

}

void WorldMap::Start()
{
	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("WorldMap_PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 1.f);
	ImageRenderer->Off();

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("WorldMap.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);
	}

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("WorldMap_Layer.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -1.f);
	}
}

void WorldMap::Update(float _deltaTime)
{
#ifdef _DEBUG
	if (GameEngineInput::GetInstance().IsKeyDown("P"))
	{
		if (ImageRenderer->IsUpdate())
		{
			ImageRenderer->Off();
		}
		else
		{
			ImageRenderer->On();
		}
	}
#endif // DEBUG
}
