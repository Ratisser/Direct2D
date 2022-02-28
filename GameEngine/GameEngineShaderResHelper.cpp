#include "PreCompile.h"
#include "GameEngineShaderResHelper.h"

#include "GameEngineTextureManager.h"

#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"

GameEngineShaderResHelper::GameEngineShaderResHelper()
{
}

GameEngineShaderResHelper::~GameEngineShaderResHelper()
{
	for (auto& Setting : AllSamplerSettings_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}

	for (auto& Setting : AllTextureSettings_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}

	AllTextureSettings_.clear();

	for (auto& Setting : AllConstantBufferSettings_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}

	AllConstantBufferSettings_.clear();
}


void GameEngineShaderResHelper::ShaderResourcesCheck(GameEngineShader* _Shader)
{
	for (auto& sampler : _Shader->GetSamplers())
	{
		GameEngineSamplerSetting* SettingData = new GameEngineSamplerSetting();
		SettingData->Shader_ = _Shader;
		SettingData->Res_ = sampler.second;
		SettingData->SettingIndex_ = sampler.first;
		auto Result = AllSamplerSettings_.insert(std::make_pair(sampler.second->GetName(), SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("���� �̸��� ���÷��� �̹� �����մϴ�." + sampler.second->GetName());
		}
	}

	for (auto& ConstantBuffer : _Shader->GetConstantBuffers())
	{
		GameEngineConstantBufferSetting* SettingData = new GameEngineConstantBufferSetting();
		SettingData->Shader = _Shader;
		SettingData->Res_ = ConstantBuffer.second;
		SettingData->SettingIndex_ = ConstantBuffer.first;
		auto Result = AllConstantBufferSettings_.insert(std::make_pair(ConstantBuffer.second->GetName(), SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("���� �̸��� ������۰� �̹� �����մϴ�." + ConstantBuffer.second->GetName());
		}
	}

	GameEngineTexture* errorTexture = GameEngineTextureManager::GetInst().Find("errorTexture.png");

	for (auto& texture : _Shader->GetTextures())
	{
		GameEngineTextureSetting* SettingData = new GameEngineTextureSetting();
		SettingData->Shader = _Shader;
		SettingData->Res_ = errorTexture;
		SettingData->SettingIndex_ = texture.first;
		auto Result = AllTextureSettings_.insert(std::make_pair(texture.second, SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("���� �̸��� �ؽ��İ� �̹� �����մϴ�." + texture.second);
		}
	}

}

void GameEngineShaderResHelper::Setting()
{
	// ������ �� �����ϱ�.
	for (auto& Setting : AllConstantBufferSettings_)
	{
		Setting.second->ChangeData();
		Setting.second->ShaderSetting();
	}

	for (auto& Setting : AllTextureSettings_)
	{
		Setting.second->ShaderSetting();
	}

	for (auto& Setting : AllSamplerSettings_)
	{
		Setting.second->ShaderSetting();
	}
}

GameEngineTexture* GameEngineShaderResHelper::SettingTexture(const std::string& _settingName, const std::string& _textureName)
{
	std::map<std::string, GameEngineTextureSetting*>::iterator findIter = AllTextureSettings_.find(_settingName);

	if (findIter == AllTextureSettings_.end())
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���ҽ� �̸��Դϴ�." + _settingName);
		return nullptr;
	}
	
	GameEngineTexture* FindTexture = GameEngineTextureManager::GetInst().Find(_textureName);

	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�. >>> " + _textureName);
		return nullptr;
	}

	findIter->second->Res_ = FindTexture;

	return FindTexture;
}

GameEngineTexture* GameEngineShaderResHelper::SettingTexture(const std::string& _textureName)
{
	std::map<std::string, GameEngineTextureSetting*>::iterator findIter = AllTextureSettings_.find("Tex");

	if (findIter == AllTextureSettings_.end())
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���ҽ� �̸��Դϴ�. Tex");
		return nullptr;
	}

	GameEngineTexture* FindTexture = GameEngineTextureManager::GetInst().Find(_textureName);

	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�. >>> " + _textureName);
		return nullptr;
	}

	findIter->second->Res_ = FindTexture;

	return FindTexture;
}

bool GameEngineShaderResHelper::IsValidConstantBuffer(const std::string& _name)
{
	std::map<std::string, GameEngineConstantBufferSetting*>::iterator findIter = AllConstantBufferSettings_.find(_name);

	if (findIter == AllConstantBufferSettings_.end())
	{
		return false;
	}

	return true;
}
