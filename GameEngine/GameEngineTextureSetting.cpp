#include "PreCompile.h"
#include "GameEngineTextureSetting.h"

GameEngineTextureSetting::GameEngineTextureSetting()
	: Res_(nullptr)
	, SettingIndex_(0)
	, Shader(nullptr)
{

}

GameEngineTextureSetting::~GameEngineTextureSetting()
{

}

void GameEngineTextureSetting::ShaderSetting()
{
	Shader->SetTexture(this);
}