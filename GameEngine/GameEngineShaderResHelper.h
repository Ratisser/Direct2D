#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "GameEngineShader.h"
#include "GameEngineConstantBufferSetting.h"
#include "GameEngineTextureSetting.h"
#include "GameEngineSamplerSetting.h"

class GameEngineTexture;
class GameEngineSampler;
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

	// 매개변수로 전달된 셰이더 개체 내부에 사용된 리소스를 체크하고 해당 리소스의 세팅 데이터를 셰이더 헬퍼 개체에 추가하는 함수
	void ShaderResourcesCheck(GameEngineShader* _Shader);

	// 현재 리소스 정보를 렌더링 파이프라인에 세팅
	void Setting();
	
	bool IsValidConstantBuffer(const std::string& _name);

	template<typename T>
	void SettingConstantBufferLink(const std::string& _SettingName, T& _Data);

	template<typename T>
	void SettingConstantBufferSet(const std::string& _SettingName, const T& _Data);

	GameEngineTexture* SettingTexture(const std::string& _settingName, const std::string& _textureName);
	GameEngineTexture* SettingTexture(const std::string& _textureName);
	GameEngineTexture* SettingTexture(const std::string& _textureName, GameEngineTexture* _texture);

private:
	std::map<std::string, GameEngineConstantBufferSetting*> AllConstantBufferSettings_;
	std::map<std::string, GameEngineTextureSetting*> AllTextureSettings_;
	std::map<std::string, GameEngineSamplerSetting*> AllSamplerSettings_;

};








template<typename T>
inline void GameEngineShaderResHelper::SettingConstantBufferLink(const std::string& _SettingName, T& _Data)
{
	std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllConstantBufferSettings_.find(_SettingName);

	if (FindIter == AllConstantBufferSettings_.end())
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
	std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllConstantBufferSettings_.find(_SettingName);

	if (FindIter == AllConstantBufferSettings_.end())
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
