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
	if (nullptr == currentMap)
	{
		return float4::ZERO;
	}

	return currentMap->ImageRenderer->GetCurrentTexture()->GetPixel(_Postion.ix(), _Postion.iy());
}