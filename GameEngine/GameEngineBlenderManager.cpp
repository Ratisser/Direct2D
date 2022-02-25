#include "PreCompile.h"
#include "GameEngineBlenderManager.h"
#include "GameEngineBlender.h"

GameEngineBlenderManager* GameEngineBlenderManager::Inst = new GameEngineBlenderManager();

GameEngineBlenderManager::GameEngineBlenderManager() // default constructer ����Ʈ ������
{

}

GameEngineBlenderManager::~GameEngineBlenderManager() // default destructer ����Ʈ �Ҹ���
{
	for (const std::pair<std::string, GameEngineBlender*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineBlender* GameEngineBlenderManager::Create(const std::string& _Name, const D3D11_BLEND_DESC& _blendDesc)
{
	GameEngineBlender* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}


	GameEngineBlender* NewRes = new GameEngineBlender();
	NewRes->SetName(_Name);
	NewRes->Create(_blendDesc);

	ResourcesMap.insert(std::map<std::string, GameEngineBlender*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineBlender* GameEngineBlenderManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineBlender*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}