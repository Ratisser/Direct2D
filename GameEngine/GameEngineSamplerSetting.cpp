#include "PreCompile.h"
#include "GameEngineSamplerSetting.h"

GameEngineSamplerSetting::GameEngineSamplerSetting()
	: Shader_(nullptr)
	, Res_(nullptr)
	, SettingIndex_(0)
{

}

GameEngineSamplerSetting::~GameEngineSamplerSetting()
{

}

void GameEngineSamplerSetting::ShaderSetting()
{
	Shader_->SetSampler(this);
}
