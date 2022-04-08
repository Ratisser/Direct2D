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
	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("BossDevil_Phase1PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 3.f);
	ImageRenderer->Off();

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("BossDevil_Phase1BackgroundTest.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 2.f);
	}

}

void DevilMap::Update(float _deltaTime)
{
}
