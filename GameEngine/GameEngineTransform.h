#pragma once

#include <GameEngineBase/GameEngineMath.h>

#include "GameEngineComponent.h"

class TransformData
{
public:
	float4x4 Wolrd;
	float4x4 View;
	float4x4 Projection;
};

class GameEngineTransform
{
public:
	GameEngineTransform(); 
	~GameEngineTransform();

	GameEngineTransform(const GameEngineTransform& _other) = delete; 
	GameEngineTransform(GameEngineTransform&& _other) = delete; 

	GameEngineTransform& operator=(const GameEngineTransform& _other) = delete;
	GameEngineTransform& operator=(const GameEngineTransform&& _other) = delete;

private:
	std::unique_ptr<TransformData> transformData_;
	std::unique_ptr<float4> scale_;
	std::unique_ptr<float4> rotation_;
	std::unique_ptr<float4> location_;
};

