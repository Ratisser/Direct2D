#include "PreCompile.h"
#include "GameEngineBlender.h"

GameEngineBlender::GameEngineBlender()
	: state_(nullptr)
	, desc_()
{

}

GameEngineBlender::~GameEngineBlender()
{
	if (nullptr != state_)
	{
		state_->Release();
		state_ = nullptr;
	}
}

void GameEngineBlender::Create(const D3D11_BLEND_DESC& _desc)
{

	ID3D11BlendState* state;
	if (S_OK != GameEngineDevice::GetDevice()->CreateBlendState(&_desc, &state))
	{
		GameEngineDebug::MsgBox("블렌더 생성에 실패했습니다.");
		return;
	}

	desc_ = _desc;
	state_ = state;
}

void GameEngineBlender::Setting()
{
	GameEngineDevice::GetContext()->OMSetBlendState(state_, nullptr, 0xffffffff);
}
