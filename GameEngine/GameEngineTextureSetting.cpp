#include "PreCompile.h"
#include "GameEngineTextureSetting.h"

GameEngineTextureSetting::GameEngineTextureSetting()
	: Res_(nullptr)
	, SettingIndex_(0)
	, Shader_(nullptr)
{

}

GameEngineTextureSetting::~GameEngineTextureSetting()
{

}

void GameEngineTextureSetting::ShaderSetting()
{
	Shader_->SetTexture(this);
}

void GameEngineTextureSetting::ShaderReset()
{
	Shader_->ResetTexture(this);
}
