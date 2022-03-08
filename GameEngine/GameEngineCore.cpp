#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineManagerHelper.h"
#include "GameEngineDevice.h"
#include "GameEngineInput.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngine/GameEngineCollision.h>

GameEngineCore* GameEngineCore::MainCore_ = nullptr;
std::map<std::string, GameEngineLevel*> GameEngineCore::allLevels_ = std::map<std::string, GameEngineLevel*>();
GameEngineLevel* GameEngineCore::nextLevel_ = nullptr;
GameEngineLevel* GameEngineCore::currentLevel_ = nullptr;

GameEngineCore::GameEngineCore() // default constructer ����Ʈ ������
{

}

GameEngineCore::~GameEngineCore() // default destructer ����Ʈ �Ҹ���
{

}

GameEngineCore::GameEngineCore(GameEngineCore&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{


}

/// <summary>
/// /////////////////////////// member
/// </summary>

void GameEngineCore::EngineInitialize()
{
	{
		GameEngineDirectory EngineTextureDir;
		EngineTextureDir.MoveParent("Direct2D");
		EngineTextureDir.MoveChild("EngineResources");
		EngineTextureDir.MoveChild("Texture");

		std::vector<GameEngineFile> AllFile = EngineTextureDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	GameEngineCollision::init();

	GameEngineSoundManager::GetInstance().Initialize();
	GameEngineInput::GetInstance();
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
	GameEngineInput::Destroy();
	GameEngineWindow::Destroy();
}

/// <summary>
/// /////////////////////////// static
/// </summary>

void GameEngineCore::MainLoop()
{
	GameEngineTime::GetInst().TimeCheck();
	GameEngineSoundManager::GetInstance().Update();
	GameEngineInput::GetInstance().update();

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
		GameEngineDebug::MsgBoxError("���� ������ �������� �ʽ��ϴ�.");
	}


	float deltaTime = GameEngineTime::GetInst().GetDeltaTime();

	currentLevel_->LevelUpdate(deltaTime);
	currentLevel_->ActorUpdate(deltaTime);
	currentLevel_->Render();
	currentLevel_->Release(deltaTime);
}

void GameEngineCore::WindowCreate(GameEngineCore& _RuntimeCore)
{
	GameEngineWindow::GetInst().CreateMainWindow("MainWindow", _RuntimeCore.StartWindowSize(), _RuntimeCore.StartWindowPos());

	// ����̽��� ��������� �մϴ�.
	// HWND �����쿡�� �����ϴ� 3D ���̺귯���ϱ� WINDOW API�� ������� ó���Ǿ� �ֽ��ϴ�.
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