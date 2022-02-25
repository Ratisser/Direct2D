#pragma once

#include "GameEngineDevice.h"

// Ό³Έν : 
class GameEngineBlender;
class GameEngineBlenderManager
{
public:
	GameEngineBlenderManager();
	~GameEngineBlenderManager();
	GameEngineBlenderManager(const GameEngineBlenderManager& _other) = delete;
	GameEngineBlenderManager(GameEngineBlenderManager&& _other) = delete;
	GameEngineBlenderManager& operator=(const GameEngineBlenderManager& _other) = delete;
	GameEngineBlenderManager& operator=(const GameEngineBlenderManager&& _other) = delete;

public:
	static GameEngineBlenderManager& GetInst()
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
	GameEngineBlender* Create(const std::string& _Name, const D3D11_BLEND_DESC& _blendDesc);
	GameEngineBlender* Find(const std::string& _Name);

private:
	static GameEngineBlenderManager* Inst;
private:
	std::map<std::string, GameEngineBlender*> ResourcesMap;
};




