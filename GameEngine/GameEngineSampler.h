#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"

class GameEngineSampler : public GameEngineObjectNameBase
{
public:
	GameEngineSampler(); 
	~GameEngineSampler();

	GameEngineSampler(const GameEngineSampler& _other) = delete; 
	GameEngineSampler(GameEngineSampler&& _other) = delete; 

	GameEngineSampler& operator=(const GameEngineSampler& _other) = delete;
	GameEngineSampler& operator=(const GameEngineSampler&& _other) = delete;
public:
	void Create(const D3D11_SAMPLER_DESC& _desc);
	ID3D11SamplerState* GetSamplerState();

private:
	D3D11_SAMPLER_DESC desc_;
	ID3D11SamplerState* state_;
};

