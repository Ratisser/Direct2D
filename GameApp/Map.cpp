#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include "Player.h"
#include "Map.h"

Map* Map::currentMap = nullptr;

Map::Map()
	: ImageRenderer(nullptr)
{
}

Map::~Map()
{
}

void Map::Start()
{
	{
		
		ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(transform_);
		ImageRenderer->SetLocation(float4{ 1000.0f, -1000.0f, 100.0f });
		ImageRenderer->SetScale(float4{ 2000.0f, 2000.0f, 1.0f });
		ImageRenderer->SetImage("Map.Png");
	}
}

void Map::levelChangeStartEvent()
{
	currentMap = this;
}

float4 Map::GetColor(GameEngineTransformComponent* _Ptr, bool YRevers /*= true*/)
{
	float4 Pos = _Ptr->GetWorldLocation();

	if (true == YRevers)
	{
		Pos.y *= -1.0f;
	}

	return GetColor(Pos);
}

float4 Map::GetColor(float4 _Postion)
{
	return float4();
	//return currentMap->ImageRenderer->GetCurrentTexture()->GetPixel(_Postion.ix(), _Postion.iy());
}