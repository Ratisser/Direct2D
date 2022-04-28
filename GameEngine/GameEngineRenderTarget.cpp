#include "PreCompile.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineTextureManager.h"
#include "GameEngineTexture.h"

GameEngineRenderTarget::GameEngineRenderTarget() // default constructer 디폴트 생성자
{

}

GameEngineRenderTarget::~GameEngineRenderTarget() // default destructer 디폴트 소멸자
{
	
}

GameEngineRenderTarget::GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

float4 GameEngineRenderTarget::GetTextureSize(size_t _Index)
{
	return Textures_[_Index]->GetTextureSize();
}

GameEngineTexture* GameEngineRenderTarget::GetTexture(size_t _Index)
{
	return Textures_[_Index];
}

ID3D11RenderTargetView* GameEngineRenderTarget::GetRenderTargetView(size_t _index)
{
	return RenderTargetViews_[_index];
}

void GameEngineRenderTarget::Clear() 
{
	for (size_t i = 0; i < RenderTargetViews_.size(); i++)
	{
		GameEngineDevice::GetContext()->ClearRenderTargetView(RenderTargetViews_[i], ClearColor_[i].Arr1D);
	}
}

void GameEngineRenderTarget::Create(const std::string _TextureName, float4 _ClearColor)
{
	GameEngineTexture* FindTexture = GameEngineTextureManager::GetInst().Find(_TextureName);
	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("FindTexture Is null Create Render Target Error");
	}

	Textures_.push_back(FindTexture);
	RenderTargetViews_.push_back(FindTexture->CreateRenderTargetView());
	ClearColor_.push_back(_ClearColor);
}

void GameEngineRenderTarget::Create(GameEngineTexture* _texture, float4 _ClearColor)
{
	GameEngineTexture* FindTexture = _texture;
	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("FindTexture Is null Create Render Target Error");
	}

	Textures_.push_back(FindTexture);
	RenderTargetViews_.push_back(FindTexture->CreateRenderTargetView());
	ClearColor_.push_back(_ClearColor); 
}

void GameEngineRenderTarget::Setting(int _Index) 
{
	if (0 >= RenderTargetViews_.size())
	{
		GameEngineDebug::MsgBoxError("Render Target Setting Error Size Zero");
	}

	if (-1 == _Index)
	{
		GameEngineDevice::GetContext()->OMSetRenderTargets(static_cast<UINT>(RenderTargetViews_.size()), &RenderTargetViews_[0], GameEngineDevice::GetDepthStencilView());
	}
	else 
	{
		GameEngineDevice::GetContext()->OMSetRenderTargets(1, &RenderTargetViews_[_Index], GameEngineDevice::GetDepthStencilView());
	}
}