#include "PreCompile.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineVertexShader.h"

GameEngineVertexShaderManager* GameEngineVertexShaderManager::Inst = new GameEngineVertexShaderManager();

GameEngineVertexShaderManager::GameEngineVertexShaderManager() // default constructer ����Ʈ ������
{

}

GameEngineVertexShaderManager::~GameEngineVertexShaderManager() // default destructer ����Ʈ �Ҹ���
{
	for (const std::pair<std::string, GameEngineVertexShader*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineVertexShaderManager::GameEngineVertexShaderManager(GameEngineVertexShaderManager&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}



GameEngineVertexShader* GameEngineVertexShaderManager::Create(const std::string& _Name, std::function<float4(const float4)> _Function)
{
	GameEngineVertexShader* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}


	GameEngineVertexShader* NewRes = new GameEngineVertexShader();
	NewRes->SetName(_Name);
	NewRes->Create(_Function);

	ResourcesMap.insert(std::map<std::string, GameEngineVertexShader*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineVertexShader* GameEngineVertexShaderManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineVertexShader* GameEngineVertexShaderManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineVertexShader* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineVertexShader* NewRes = new GameEngineVertexShader();
	NewRes->SetName(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineVertexShader*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineVertexShader* GameEngineVertexShaderManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineVertexShader*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}