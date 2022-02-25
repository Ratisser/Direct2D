#include "PreCompile.h"
#include "GameEngineSampler.h"

GameEngineSampler::GameEngineSampler()
	: state_(nullptr)
	, desc_()
{

}

GameEngineSampler::~GameEngineSampler()
{
	if (nullptr != state_)
	{
		state_->Release();
		state_ = nullptr;
	}
}

void GameEngineSampler::Create(const D3D11_SAMPLER_DESC& _desc)
{
	desc_ = _desc;

	if (S_OK != GameEngineDevice::GetDevice()->CreateSamplerState(&desc_, &state_))
	{
		GameEngineDebug::MsgBox("���÷� ������ �����߽��ϴ�.");
		return;
	}
}
