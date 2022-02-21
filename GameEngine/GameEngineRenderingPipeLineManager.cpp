#include "PreCompile.h"
#include "GameEngineRenderingPipelineManager.h"
#include "GameEngineRenderingPipeline.h"

GameEngineRenderingPipelineManager* GameEngineRenderingPipelineManager::Inst = new GameEngineRenderingPipelineManager();

GameEngineRenderingPipelineManager::GameEngineRenderingPipelineManager() // default constructer ����Ʈ ������
{

}

GameEngineRenderingPipelineManager::~GameEngineRenderingPipelineManager() // default destructer ����Ʈ �Ҹ���
{
	for (const std::pair<std::string, GameEngineRenderingPipeline*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineRenderingPipelineManager::GameEngineRenderingPipelineManager(GameEngineRenderingPipelineManager&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}



GameEngineRenderingPipeline* GameEngineRenderingPipelineManager::Create(const std::string& _Name)
{
	GameEngineRenderingPipeline* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineRenderingPipeline* NewRes = new GameEngineRenderingPipeline();
	NewRes->SetName(_Name);

	// �׸��� ���Ұų�?

	ResourcesMap.insert(std::map<std::string, GameEngineRenderingPipeline*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineRenderingPipeline* GameEngineRenderingPipelineManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineRenderingPipeline* GameEngineRenderingPipelineManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineRenderingPipeline* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineRenderingPipeline* NewRes = new GameEngineRenderingPipeline();
	NewRes->SetName(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineRenderingPipeline*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineRenderingPipeline* GameEngineRenderingPipelineManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineRenderingPipeline*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}