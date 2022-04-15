#include "PreCompile.h"
#include "WorldMap.h"
#include <GameEngine\GameEngineInput.h>

#include <GameEngine\GameEngineCore.h>

#include "WorldMapPlayer.h"

WorldMap::WorldMap()
{

}

WorldMap::~WorldMap()
{

}

void WorldMap::Start()
{
	Map::Start();

	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("WorldMap_PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 5.f);
	ImageRenderer->Off();

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("WorldMap.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 6.f);
	}

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("WorldMap_Layer.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);
	}
}

void WorldMap::Update(float _deltaTime)
{
	if (GameEngineInput::GetInstance().IsKeyDown("P"))
	{
		GameEngineCore::ChangeLevel("DevilLevel");
	}

	if (GameEngineInput::GetInstance().IsKeyDown("O"))
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
}
