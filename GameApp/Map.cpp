#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>

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

void Map::Start()
{
	{
		GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
		float4 color;
		color.r = 1.f;
		color.g = 1.f;
		color.b = 1.f;
		color.a = 0.2f;
		r->SetLocationZ(1.0f);
		r->SetColor(color);
		r->CreateAnimationFolder("OldFilmEffect", 0.034f, true, false);
		r->SetScale(1280.f, 720.f);
		r->ChangeAnimation("OldFilmEffect");
	}
	//{
	//	GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	//	float4 color;
	//	color.r = 0.2f;
	//	color.g = 0.2f;
	//	color.b = 0.2f;
	//	color.a = 0.2f;
	//	r->SetColor(color);
	//	r->CreateAnimationFolder("OldFilmEffect", 0.034f, true, false);
	//	r->SetScale(1280.f, 720.f);
	//	r->SetLocationZ(-6.0f);
	//	r->ChangeAnimation("OldFilmEffect");
	//}
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