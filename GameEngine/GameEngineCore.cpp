#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineResourcesManager.h"
#include "GameEngineDevice.h"

GameEngineCore* GameEngineCore::MainCore_ = nullptr;
std::map<std::string, GameEngineLevel*> GameEngineCore::allLevels_ = std::map<std::string, GameEngineLevel*>();
GameEngineLevel* GameEngineCore::nextLevel_ = nullptr;
GameEngineLevel* GameEngineCore::currentLevel_ = nullptr;

GameEngineCore::GameEngineCore() // default constructer 디폴트 생성자
{

}

GameEngineCore::~GameEngineCore() // default destructer 디폴트 소멸자
{

}

GameEngineCore::GameEngineCore(GameEngineCore&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{


}

/// <summary>
/// /////////////////////////// member
/// </summary>

void GameEngineCore::EngineInitialize()
{
	GameEngineSoundManager::GetInstance().Initialize();
}


void GameEngineCore::EngineDestroy()
{
	for (std::pair<const std::string, GameEngineLevel*>& p : allLevels_)
	{
		if (nullptr != p.second)
		{
			delete p.second;
			p.second = nullptr;
		}
	}

	GameEngineManagerHelper::ManagerRelease();
	GameEngineTime::Destroy();
	GameEngineDevice::Destroy();
	GameEngineWindow::Destroy();
}

/// <summary>
/// /////////////////////////// static
/// </summary>

void GameEngineCore::MainLoop()
{
	GameEngineTime::GetInst().TimeCheck();
	GameEngineSoundManager::GetInstance().Update();

	if (nullptr != nextLevel_)
	{
		if (nullptr != currentLevel_)
		{
			currentLevel_->LevelChangeEndEvent();
		}

		nextLevel_->LevelChangeStartEvent();
		currentLevel_ = nextLevel_;
		nextLevel_ = nullptr;
		GameEngineTime::GetInst().TimeCheck();
	}

	if (nullptr == currentLevel_)
	{
		GameEngineDebug::MsgBoxError("현재 레벨이 존재하지 않습니다.");
	}

	currentLevel_->LevelUpdate(GameEngineTime::GetInst().GetDeltaTime());
	currentLevel_->ActorUpdate(GameEngineTime::GetInst().GetDeltaTime());
}

void GameEngineCore::WindowCreate(GameEngineCore& _RuntimeCore)
{
	GameEngineWindow::GetInst().CreateMainWindow("MainWindow", _RuntimeCore.StartWindowSize(), _RuntimeCore.StartWindowPos());

	// 디바이스가 만들어져야 합니다.
	// HWND 윈도우에서 제공하는 3D 라이브러리니까 WINDOW API를 기반으로 처리되어 있습니다.
	GameEngineDevice::GetInst().Initialize();
}

void GameEngineCore::Loop()
{
	GameEngineWindow::GetInst().Loop(&GameEngineCore::MainLoop);
}


void GameEngineCore::ChangeLevel(const std::string& _levelName)
{
	std::map<std::string, GameEngineLevel*>::iterator findIter = allLevels_.find(_levelName);
	
	if (findIter == allLevels_.end())
	{
		GameEngineDebug::MsgBoxError("There is no level named \"" + _levelName + "\"");
	}

	nextLevel_ = findIter->second;
}