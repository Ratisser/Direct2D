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
	GameEngineConstantBufferSetting();
	~GameEngineConstantBufferSetting();

public:
	void Clear();
	void ChangeData();
	void ShaderSetting();
	void ShaderReset();

public:
	GameEngineShader* Shader;
	GameEngineConstantBuffer* Res_;
	SettingMode Mode_;
	char* SettingData_;
	size_t SettingDataSize_;
	int SettingIndex_;
	bool IsSet_;
};