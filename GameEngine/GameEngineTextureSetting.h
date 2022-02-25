#pragma once

#include "GameEngineShader.h"
#include "GameEngineTexture.h"

class GameEngineTextureSetting
{
public:
	GameEngineShader* Shader;
	GameEngineTexture* Res_;
	int SettingIndex_;

public:
	void ShaderSetting()
	{
		Shader->SetTexture(this);
	}

public:
	GameEngineTextureSetting()
		: Res_(nullptr)
		, SettingIndex_(0)
		, Shader(nullptr)
	{

	}

	~GameEngineTextureSetting()
	{
		
	}
};
