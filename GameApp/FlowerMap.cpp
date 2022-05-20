#include "PreCompile.h"
#include "FlowerMap.h"

#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineInput.h>

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineCore.h>

#include "eCollisionGroup.h"
#include <GameApp\FadeOut.h>

FlowerMap::FlowerMap()
	: cloud1_(nullptr)
	, cloud2_(nullptr)
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
		cloud1_ = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		cloud1_->SetTexture("Flower_Background_Cloud01.png", true);
		float4 textureSize = cloud1_->GetCurrentTexture()->GetTextureSize();
		cloud1_->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 4.1f);
		cloud1_->AddLocation(0.0f, -200.f);

	}

	{
		cloud2_ = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		cloud2_->SetTexture("Flower_Background_Cloud02.png", true);
		float4 textureSize = cloud2_->GetCurrentTexture()->GetTextureSize();
		cloud2_->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, 5.0f);
	}

	{
		GameEngineImageRenderer* mapObjects = CreateTransformComponent<GameEngineImageRenderer>(nullptr);
		mapObjects->SetTexture("Flower_Background_Front.png", true);
		float4 textureSize = mapObjects->GetCurrentTexture()->GetTextureSize();
		mapObjects->SetLocation(textureSize.x / 2.f, -textureSize.y / 2.f, -1.f);
		mapObjects->AddLocation(-30.0f, -480.f);
	}

}

void FlowerMap::Update(float _deltaTime)
{
	cloud1_->AddAddUV({ 0.02f * _deltaTime, 0.0f });
	cloud2_->AddAddUV({ 0.01f * _deltaTime, 0.0f });
}
