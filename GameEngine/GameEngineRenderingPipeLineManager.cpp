#include "PreCompile.h"
#include "GameEngineRenderingPipelineManager.h"
#include "GameEngineRenderingPipeline.h"

GameEngineRenderingPipelineManager* GameEngineRenderingPipelineManager::Inst = new GameEngineRenderingPipelineManager();

GameEngineRenderingPipelineManager::GameEngineRenderingPipelineManager() // default constructer 디폴트 생성자
{

}

GameEngineRenderingPipelineManager::~GameEngineRenderingPipelineManager() // default destructer 디폴트 소멸자
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

GameEngineRenderingPipelineManager::GameEngineRenderingPipelineManager(GameEngineRenderingPipelineManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
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

	// 그리고 뭘할거냐?

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