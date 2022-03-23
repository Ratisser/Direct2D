#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine\GameEngineImageRenderer.h>

class GameEngineImageRenderer;
class Map : public GameEngineActor
{
public:
	Map();
	~Map();
	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

public:
	static float4 GetColor(GameEngineTransformComponent* _Ptr, bool YRevers = true);
	static float4 GetColor(float4 _Postion);

public:
	GameEngineImageRenderer* ImageRenderer;

protected:
	void levelChangeStartEvent() override;

private:
	static Map* currentMap;
};