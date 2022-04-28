#include "PreCompile.h"
#include "GameEngineConstantBufferSetting.h"

void GameEngineConstantBufferSetting::Clear()
{
	if (SettingMode::Set == Mode_)
	{
		if (nullptr != SettingData_)
		{
			delete[] SettingData_;
			SettingData_ = nullptr;
		}
	}
}

void GameEngineConstantBufferSetting::ChangeData()
{
	Res_->ChangeData(SettingData_, SettingDataSize_);
}

void GameEngineConstantBufferSetting::ShaderSetting()
{
	Shader->SetConstantBuffers(this);
}

void GameEngineConstantBufferSetting::ShaderReset()
{
	Shader->ResetConstantBuffers(this);
}

GameEngineConstantBufferSetting::GameEngineConstantBufferSetting()
	: SettingData_(nullptr)
	, Mode_(SettingMode::MAX)
	, IsSet_(false)
	, Res_(nullptr)
	, SettingDataSize_(0)
	, SettingIndex_(0)
	, Shader(nullptr)
{

}

GameEngineConstantBufferSetting::~GameEngineConstantBufferSetting()
{
	Clear();
}

