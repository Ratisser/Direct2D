#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "GameEngineConstantBufferSetting.h"
#include "GameEngineTextureSetting.h"
#include "GameEngine/GameEngineShader.h"



class GameEngineShader;
class GameEngineConstantBuffer;
class GameEngineShaderResHelper
{
public:
	GameEngineShaderResHelper();
	~GameEngineShaderResHelper();

	GameEngineShaderResHelper(const GameEngineShaderResHelper& _Other) = delete;
	GameEngineShaderResHelper(GameEngineShaderResHelper&& _Other) noexcept = delete;
	GameEngineShaderResHelper& operator=(const GameEngineShaderResHelper& _Other) = delete;
	GameEngineShaderResHelper& operator=(GameEngineShaderResHelper&& _Other) noexcept = delete;

	void ShaderResourcesCheck(GameEngineShader* _Shader);
	void Setting();

	bool IsValidConstantBuffer(const std::string& _name);

	template<typename T>
	void SettingConstantBufferLink(const std::string& _SettingName, T& _Data);

	template<typename T>
	void SettingConstantBufferSet(const std::string& _SettingName, const T& _Data);

private:
	std::map<std::string, GameEngineConstantBufferSetting*> AllConstantBuffers_;

};




template<typename T>
inline void GameEngineShaderResHelper::SettingConstantBufferLink(const std::string& _SettingName, T& _Data)
{
	std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllConstantBuffers_.find(_SettingName);

	if (FindIter == AllConstantBuffers_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 상수버퍼를 세팅하려고 했습니다." + _SettingName);
		return;
	}

	GameEngineConstantBufferSetting* SettingData = FindIter->second;

	SettingData->Mode_ = SettingMode::Link;
	SettingData->SettingDataSize_ = sizeof(_Data);
	SettingData->SettingData_ = reinterpret_cast<char*>(&_Data);
}

template<typename T>
inline void GameEngineShaderResHelper::SettingConstantBufferSet(const std::string& _SettingName, const T& _Data)
{
	std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllConstantBuffers_.find(_SettingName);

	if (FindIter == AllConstantBuffers_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 상수버퍼를 세팅하려고 했습니다." + _SettingName);
		return;
	}

	GameEngineConstantBufferSetting* SettingData = FindIter->second;

	// 최초세팅이라면 지워준다.
	if (SettingData->Mode_ == SettingMode::MAX)
	{
		SettingData->Clear();
	}

	SettingData->Mode_ = SettingMode::Set;
	SettingData->SettingDataSize_ = sizeof(_Data);

	if (nullptr != SettingData->SettingData_)
	{
		delete[] SettingData->SettingData_;
	}

	SettingData->SettingData_ = new char[sizeof(_Data)];

	memcpy_s(SettingData->SettingData_, sizeof(_Data), &_Data, sizeof(_Data));
}
