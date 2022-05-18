#include "PreCompile.h"
#include "FlowerMap.h"

#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineInput.h>

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineCore.h>

#include "eCollisionGroup.h"
#include <GameApp\FadeOut.h>

FlowerMap::FlowerMap()
{

}

FlowerMap::~FlowerMap()
{

}

void FlowerMap::Start()
{
	Map::Start();

	ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
	ImageRenderer->SetTexture("Flower_Background_PixelCheck.png", true);
	float4 textureSize = ImageRenderer->GetCurrentTexture()->GetTextureSize();
	ImageRenderer->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -2.f);
	ImageRenderer->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	ImageRenderer->Off();

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("Flower_Background.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 4.f);
	}

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("Flower_Background_Cloud01.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 4.1f);
		mapObjects->AddLocation(0.0f, -200.f);

	}

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("Flower_Background_Cloud02.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 5.0f);
	}

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("Flower_Background_Front.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -1.f);
		mapObjects->AddLocation(0.0f, -480.f);
	}

}

void FlowerMap::Update(float _deltaTime)
{
}
