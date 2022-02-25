#pragma once

#include "GameEngineDevice.h"

// Ό³Έν : 
class GameEngineSampler;
class GameEngineSamplerManager
{
public:
	GameEngineSamplerManager();
	~GameEngineSamplerManager();
	GameEngineSamplerManager(const GameEngineSamplerManager& _other) = delete;
	GameEngineSamplerManager(GameEngineSamplerManager&& _other) = delete;
	GameEngineSamplerManager& operator=(const GameEngineSamplerManager& _other) = delete;
	GameEngineSamplerManager& operator=(const GameEngineSamplerManager&& _other) = delete;

public:
	static GameEngineSamplerManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

public:
	GameEngineSampler* Create(const std::string& _Name, const D3D11_SAMPLER_DESC& _samplerDesc);
	GameEngineSampler* Find(const std::string& _Name);

private:
	static GameEngineSamplerManager* Inst;
private:
	std::map<std::string, GameEngineSampler*> ResourcesMap;
};




