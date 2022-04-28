#include "PreCompile.h"
#include <GameEngineBase\GameEngineTime.h>
#include <GameEngine\GameEngineWindow.h>


#include "GameEngineLevelControlWindow.h"
#include "GameEngineCore.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"

GameEngineLevelControlWindow::GameEngineLevelControlWindow()
	: volume_(50.f)
	, prevVolume_(50.f)
	, fps_(0)
{
}

GameEngineLevelControlWindow::~GameEngineLevelControlWindow()
{
}

void GameEngineLevelControlWindow::OnGUI()
{
	static float Acc = 0.0f;
	
	Acc += GameEngineTime::GetInst().GetDeltaTime();
	if (1.0 <= Acc)
	{
		fps_ = 1.0f / ImGui::GetIO().DeltaTime;
		Acc = 0.0f;
	}

	//ImGui::Text("UI FPS : %.2f", fps_);
	ImGui::NextColumn();

	ImGui::Text("Game FPS : %.2f", GameEngineWindow::GetFPS());
	ImGui::NextColumn();

	ImGui::Text("LevelSelect");
	ImGui::NextColumn();


	int Count = static_cast<int>(GameEngineCore::allLevels_.size());

	for (auto& Level : GameEngineCore::allLevels_)
	{
		if (true == ImGui::Button(Level.first.c_str()))
		{
			GameEngineCore::ChangeLevel(Level.first);
		}

		--Count;

		if (Count != 0)
		{
			ImGui::NextColumn();
		}
	}

	ImGui::Text("Volume");
	ImGui::NextColumn();
	ImGui::SliderFloat("%", &volume_, 0.0f, 100.f, "%.2f");

	if (prevVolume_ != volume_)
	{
		GameEngineSoundManager::GetInstance().SetGlobalVolume(volume_ / 100.f);
		prevVolume_ = volume_;
	}
}

