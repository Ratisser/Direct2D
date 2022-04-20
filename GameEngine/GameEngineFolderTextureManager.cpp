#include "PreCompile.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineFolderTexture.h"
#include <GameEngine\GameEngineCore.h>

GameEngineFolderTextureManager* GameEngineFolderTextureManager::Inst = new GameEngineFolderTextureManager();
std::mutex GameEngineFolderTextureManager::managerLock_;

GameEngineFolderTextureManager::GameEngineFolderTextureManager() // default constructer 디폴트 생성자
{

}

GameEngineFolderTextureManager::~GameEngineFolderTextureManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineFolderTexture*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineFolderTextureManager::GameEngineFolderTextureManager(GameEngineFolderTextureManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

GameEngineFolderTexture* GameEngineFolderTextureManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineFolderTexture* GameEngineFolderTextureManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineFolderTexture* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFolderTexture* NewRes = new GameEngineFolderTexture();
	NewRes->SetName(_Name);
	NewRes->Load(_Path);
	{
		std::lock_guard lock(managerLock_);
		ResourcesMap.insert(std::map<std::string, GameEngineFolderTexture*>::value_type(_Name, NewRes));
	}
	return NewRes;
}

GameEngineFolderTexture* GameEngineFolderTextureManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineFolderTexture*>::iterator FindIter;

	{
		std::lock_guard lock(managerLock_);
		FindIter = ResourcesMap.find(_Name);
	}

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}