#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"

GameEngineCore* GameEngineCore::MainCore = nullptr;

GameEngineCore::GameEngineCore() // default constructer 디폴트 생성자
{

}

GameEngineCore::~GameEngineCore() // default destructer 디폴트 소멸자
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
	GameEngineDirectXDevice::GetInst().Initialize();
}

void GameEngineCore::run() 
{
	GameEngineWindow::GetInst().Loop(&GameEngineCore::mainLoop);
}



void GameEngineCore::releaseEngine()
{
	GameEngineManagerHelper::ManagerRelase();
	GameEngineDirectXDevice::Destroy();
	GameEngineWindow::Destroy();
	GameEngineSound::Destroy();
	GameEngineTime::Destroy();
}