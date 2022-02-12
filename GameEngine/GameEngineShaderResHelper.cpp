#include "PreCompile.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineConstantBuffer.h"

GameEngineShaderResHelper::GameEngineShaderResHelper() 
{
}

GameEngineShaderResHelper::~GameEngineShaderResHelper() 
{

	for (auto& Setting : AllSettingData_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}
}


void GameEngineShaderResHelper::ShaderResourcesCheck(GameEngineShader* _Shader) 
{
	// _Shader는 상수버퍼를 들고 있지
	// 이 상수버퍼를 각 플레이어나 몬스터가 세팅해줬는지 안했는지는 
	// GameEngineConstantBufferSetting이 가지게 됩니다.

	for (auto& ConstantBuffer : _Shader->GetConstantBuffers())
	{
		GameEngineConstantBufferSetting* SettingData = new GameEngineConstantBufferSetting();
		SettingData->Shader = _Shader;
		SettingData->Res_ = ConstantBuffer.second;
		SettingData->SettingIndex_ = ConstantBuffer.first;
		auto Result = AllSettingData_.insert(std::make_pair(ConstantBuffer.second->GetName(), SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("같은 이름의 상수버퍼가 이미 존재합니다." + ConstantBuffer.second->GetName());
		}
	}
}

void GameEngineShaderResHelper::Setting() 
{
	// 정보가 다 있으니까.
	for (auto& Setting : AllSettingData_)
	{
		Setting.second->ChangeData();
		Setting.second->ShaderSetting();
	}

}