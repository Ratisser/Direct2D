#pragma once
#include <GameEngine\GameEngineGUI.h>
#include <GameEngine\GameEngineRenderTarget.h>

// Ό³Έν :
class GameEngineLevelControlWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	GameEngineLevelControlWindow();
	~GameEngineLevelControlWindow();

	// delete Function
	GameEngineLevelControlWindow(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow(GameEngineLevelControlWindow&& _Other) noexcept = delete;
	GameEngineLevelControlWindow& operator=(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow& operator=(GameEngineLevelControlWindow&& _Other) noexcept = delete;

protected:
	void OnGUI() override;


private:
	float fps_;
	float volume_;
	float prevVolume_;
	bool bPostProcess_;
	bool bPrevPostProcess_;
};

