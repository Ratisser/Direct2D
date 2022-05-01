#pragma once
#include <GameEngine\GameEngineGUI.h>
#include <GameEngine\GameEngineRenderTarget.h>

// Ό³Έν :
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

