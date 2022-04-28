#pragma once

#include "GameEngineShader.h"
#include "GameEngineTexture.h"

class GameEngineTextureSetting
{
public:
	GameEngineTextureSetting();
	~GameEngineTextureSetting();
public:
	void ShaderSetting();
	void ShaderReset();

public:
	GameEngineShader* Shader_;
	GameEngineTexture* Res_;
	int SettingIndex_;
};
