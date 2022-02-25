#include "PreCompile.h"
#include "GameEngineSamplerManager.h"
#include "GameEngineSampler.h"

GameEngineSamplerManager* GameEngineSamplerManager::Inst = new GameEngineSamplerManager();

GameEngineSamplerManager::GameEngineSamplerManager() // default constructer 디폴트 생성자
{

}

GameEngineSamplerManager::~GameEngineSamplerManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineSampler*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineSampler* GameEngineSamplerManager::Create(const std::string& _Name, const D3D11_SAMPLER_DESC& _samplerDesc)
{
	GameEngineSampler* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}


	GameEngineSampler* NewRes = new GameEngineSampler();
	NewRes->SetName(_Name);
	NewRes->Create(_samplerDesc);

	ResourcesMap.insert(std::map<std::string, GameEngineSampler*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineSampler* GameEngineSamplerManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineSampler*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}