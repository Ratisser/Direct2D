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
	ID3D11SamplerState* state;
	if (S_OK != GameEngineDevice::GetDevice()->CreateSamplerState(&_desc, &state))
	{
		GameEngineDebug::MsgBoxError("샘플러 생성에 실패했습니다.");
		return;
	}

	desc_ = _desc;
	state_ = state;
}

ID3D11SamplerState* GameEngineSampler::GetSamplerState()
{
	return state_;
}
