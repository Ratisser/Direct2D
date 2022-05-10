#pragma once
#include <GameEngine\GameEngineGUI.h>
#include <GameEngine\GameEngineRenderTarget.h>

class GameEngineLevelControlWindow : public GameEngineGUIWindow
{
public:
	GameEngineLevelControlWindow();
	~GameEngineLevelControlWindow();
	GameEngineLevelControlWindow(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow(GameEngineLevelControlWindow&& _Other) noexcept = delete;
	GameEngineLevelControlWindow& operator=(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow& operator=(GameEngineLevelControlWindow&& _Other) noexcept = delete;

public:
	/* AddText 사용 예
	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("블라블라");
	}
	*/
	void AddText(const std::string& _str);

protected:
	void OnGUI() override;


private:
	std::vector<std::string> texts_;

	float fps_;
	float volume_;
	float prevVolume_;
	bool bPostProcess_;
	bool bPrevPostProcess_;
};

