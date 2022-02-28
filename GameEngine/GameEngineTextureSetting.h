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

public:
	GameEngineShader* Shader;
	GameEngineTexture* Res_;
	int SettingIndex_;
};
