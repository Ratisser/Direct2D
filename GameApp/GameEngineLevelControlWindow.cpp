#include "PreCompile.h"
#include "GameEngineLevelControlWindow.h"

#include <GameEngineBase\GameEngineTime.h>
#include <GameEngine\GameEngineInput.h>
#include <GameEngine\GameEngineWindow.h>
#include <GameEngine\GameEngineCore.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameEngine\GameEngineActor.h>

#include "DevilLevel.h"
#include "Devil.h"

GameEngineLevelControlWindow::GameEngineLevelControlWindow()
	: volume_(50.f)
	, prevVolume_(50.f)
	, fps_(0)
	, bPostProcess_(true)
	, bPrevPostProcess_(true)
{
}

GameEngineLevelControlWindow::~GameEngineLevelControlWindow()
{
}

void GameEngineLevelControlWindow::AddText(const std::string& _str)
{
	texts_.push_back(_str);
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

	float4 mousePos = GameEngineInput::GetInstance().GetMousePosition();
	ImGui::Text("Mouse X, Y : %d, %d", mousePos.ix(), mousePos.iy());
	ImGui::NextColumn();

	GameEngineActor* camera = GameEngineCore::currentLevel_->GetMainCameraActor();
	float4 cameraPos = float4::ZERO;

	if (nullptr != camera)
	{
		cameraPos = camera->GetTransform()->GetWorldLocation();
	}
	ImGui::Text("World Mouse X, Y : %d, %d", mousePos.ix() + cameraPos.ix(), mousePos.iy() + cameraPos.iy());
	ImGui::NextColumn();

	{
		GameEngineCameraComponent* camera = GameEngineCore::currentLevel_->GetFreeCameraActor()->GetCameraComponent();
		float4 cameraPos = float4::ZERO;

		if (nullptr != camera)
		{
			cameraPos = camera->GetWorldLocation();
		}
		ImGui::Text("World Mouse by FC X, Y : %d, %d", mousePos.ix() + cameraPos.ix(), mousePos.iy() + cameraPos.iy());
		ImGui::NextColumn();
	}

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

	ImGui::NextColumn();
	ImGui::NextColumn();
	
	ImGui::Checkbox("PostProcess", &GameEngineLevel::PostProcess_);
	ImGui::NextColumn();


	if (GameEngineCore::currentLevel_->GetName() == "DevilLevel")
	{
		DevilLevel* devilLevel = dynamic_cast<DevilLevel*>(GameEngineCore::currentLevel_);
		if (nullptr != devilLevel)
		{
			ImGui::Text("Devil HP : %d", devilLevel->GetDevil()->GetHP());
			ImGui::NextColumn();
		}
	}

	for (const std::string& str : texts_)
	{
		ImGui::Text(str.c_str());
		ImGui::NextColumn();
	}

	texts_.clear();
}

