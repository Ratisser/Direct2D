#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "GameEngineRenderingSettingData.h"
#include "GameEngine/GameEngineShader.h"



class GameEngineShader;
class GameEngineConstantBuffer;
// 설명 :
class GameEngineShaderResHelper
{
	/////////////////////////////////// 쉐이더 세팅 데이터들

public:
	// constrcuter destructer
	GameEngineShaderResHelper();
	~GameEngineShaderResHelper();

	// delete Function
	GameEngineShaderResHelper(const GameEngineShaderResHelper& _Other) = delete;
	GameEngineShaderResHelper(GameEngineShaderResHelper&& _Other) noexcept = delete;
	GameEngineShaderResHelper& operator=(const GameEngineShaderResHelper& _Other) = delete;
	GameEngineShaderResHelper& operator=(GameEngineShaderResHelper&& _Other) noexcept = delete;

	void ShaderResourcesCheck(GameEngineShader* _Shader);

	void Setting();

	// 그 주소값을 지속적으로 계속 세팅해주는 모드를 말한다.
	// 한번 세팅해주면 넣어준 주소값의 위치가 파괴되기전까지는 여러분들은 신경쓸 필요가 없다.

	template<typename T>
	void SettingConstantBufferLink(const std::string& _SettingName, T& _Data) 
	{
		std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllSettingData_.find(_SettingName);

		if (FindIter == AllSettingData_.end())
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 상수버퍼를 세팅하려고 했습니다." + _SettingName);
			return;
		}

		GameEngineConstantBufferSetting* SettingData = FindIter->second;

		SettingData->Mode_ = SettingMode::Link;
		SettingData->SettingDataSize_ = sizeof(_Data);
		SettingData->SettingData_ = reinterpret_cast<char*>(&_Data);
	}

	// 동적할당을 해서 같은 크기의 복사본을 만들어 냅니다.
	// 한번 세팅해주면 세팅해준 순간의 값으로 고정되는겁니다.
	template<typename T>
	void SettingConstantBufferSet(const std::string& _SettingName, const T& _Data)
	{
		std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllSettingData_.find(_SettingName);

		if (FindIter == AllSettingData_.end())
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 상수버퍼를 세팅하려고 했습니다." + _SettingName);
			return;
		}

		GameEngineConstantBufferSetting* SettingData = FindIter->second;

		// 최초세팅이라면 지워준다.
		//if (SettingData->Mode_ == SettingMode::MAX)
		//{
		//	SettingData->Clear();
		//}

		SettingData->Mode_ = SettingMode::Set;
		SettingData->SettingDataSize_ = sizeof(_Data);

		if (nullptr == SettingData->SettingData_)
		{
			SettingData->SettingData_ = new char[sizeof(_Data)];
		}

		memcpy_s(SettingData->SettingData_, sizeof(_Data), &_Data, sizeof(_Data));
	}

protected:

private:
	std::map<std::string, GameEngineConstantBufferSetting*> AllSettingData_;


};

