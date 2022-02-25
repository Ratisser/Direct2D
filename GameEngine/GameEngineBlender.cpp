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
	desc_ = _desc;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBlendState(&desc_, &state_))
	{
		GameEngineDebug::MsgBox("블렌더 생성에 실패했습니다.");
		return;
	}
}
