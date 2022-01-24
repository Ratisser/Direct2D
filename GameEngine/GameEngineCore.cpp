#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"

GameEngineCore* GameEngineCore::MainCore = nullptr;

GameEngineCore::GameEngineCore() // default constructer ����Ʈ ������
{

}

GameEngineCore::~GameEngineCore() // default destructer ����Ʈ �Ҹ���
{

}

void GameEngineCore::mainLoop() 
{
	GameEngineTime::GetInst().UpdateTime();
	GameEngineSound::GetInst().Update();

	MainCore->GameLoop();
}

void GameEngineCore::initializeEngine()
{
	GameEngineDebug::LeakCheckOn();

	GameEngineSound::GetInst().Initialize();
	GameEngineWindow::GetInst().CreateMainWindow("MainWindow", { 1280, 720 }, {0, 0});
}

void GameEngineCore::run() 
{
	GameEngineWindow::GetInst().Loop(&GameEngineCore::mainLoop);
}



void GameEngineCore::releaseEngine()
{
	GameEngineManagerHelper::ManagerRelase();
	GameEngineWindow::Destroy();
	GameEngineSound::Destroy();
	GameEngineTime::Destroy();
}