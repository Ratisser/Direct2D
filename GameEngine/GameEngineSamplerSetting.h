#pragma once

#include "GameEngineSampler.h"
#include "GameEngineShader.h"

class GameEngineSamplerSetting
{
public:
	GameEngineSamplerSetting(); 
	~GameEngineSamplerSetting();
	GameEngineSamplerSetting(const GameEngineSamplerSetting& _other) = delete; 
	GameEngineSamplerSetting(GameEngineSamplerSetting&& _other) = delete; 
	GameEngineSamplerSetting& operator=(const GameEngineSamplerSetting& _other) = delete;
	GameEngineSamplerSetting& operator=(const GameEngineSamplerSetting&& _other) = delete;

public:
	void ShaderSetting();
	void ShaderReset();

public:
	GameEngineShader* Shader_;
	GameEngineSampler* Res_;
	int SettingIndex_;
};

