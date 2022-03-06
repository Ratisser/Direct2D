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

	// �Ű������� ���޵� ���̴� ��ü ���ο� ���� ���ҽ��� üũ�ϰ� �ش� ���ҽ��� ���� �����͸� ���̴� ���� ��ü�� �߰��ϴ� �Լ�
	void ShaderResourcesCheck(GameEngineShader* _Shader);

	// ���� ���ҽ� ������ ������ ���������ο� ����
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
		GameEngineDebug::MsgBoxError("�������� �ʴ� ������۸� �����Ϸ��� �߽��ϴ�." + _SettingName);
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
		GameEngineDebug::MsgBoxError("�������� �ʴ� ������۸� �����Ϸ��� �߽��ϴ�." + _SettingName);
		return;
	}

	GameEngineConstantBufferSetting* SettingData = FindIter->second;

	// ���ʼ����̶�� �����ش�.
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
