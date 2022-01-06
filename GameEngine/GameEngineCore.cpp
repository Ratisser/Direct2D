#include "PreCompile.h"
#include "GameEngineCore.h"

#include <GameEngineBase/GameEngineSoundManager.h>
#include <GameEngineBase/GameEngineSoundPlayer.h>
#include <GameEngineBase/GameEngineTimer.h>

GameEngineCore::GameEngineCore()
{

}

GameEngineCore::~GameEngineCore()
{

}

void GameEngineCore::Initialize()
{
	GameEngineTimer::GetInstance().UpdateTime();
	GameEngineSoundManager::GetInstance().Initialize();

	loadResources();
}

void GameEngineCore::Run()
{
	while (false == IsDeath())
	{
		GameEngineTimer::GetInstance().UpdateTime();
		GameEngineSoundManager::GetInstance().Update();
		GameEngineSoundManager::GetInstance().Update();

		static GameEngineSoundPlayer player("stage1-1.mid");

		if (false == player.IsPlaying())
		{
			player.Play();
		}
	}
}

void GameEngineCore::Release()
{
	release();
	GameEngineSoundManager::Destroy();
	GameEngineTimer::Destroy();
}
