#pragma once
#include "GameEngineShader.h"
#include "GameEngineConstantBuffer.h"

enum class SettingMode
{
	Link,
	Set,
	MAX,
};



class GameEngineShader;
class GameEngineConstantBuffer;
class GameEngineConstantBufferSetting
{
public:
	GameEngineShader* Shader;
	GameEngineConstantBuffer* Res_;
	SettingMode Mode_;
	char* SettingData_;
	size_t SettingDataSize_;
	int SettingIndex_;
	// 이번프레임에 setting을 해줬느냐 말았느냐.
	bool IsSet_;

public:
	void Clear() 
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

	void ChangeData() 
	{
		Res_->ChangeData(SettingData_, SettingDataSize_);
	}

	void ShaderSetting()
	{
		Shader->SetConstantBuffers(this);
	}

public:
	GameEngineConstantBufferSetting()
		: SettingData_(nullptr)
		, Mode_(SettingMode::MAX)
		, IsSet_(false)
		, Res_(nullptr)
		, SettingDataSize_(0)
		, SettingIndex_(0)
		, Shader(nullptr)
	{

	}

	~GameEngineConstantBufferSetting() 
	{
		Clear();
	}
};
