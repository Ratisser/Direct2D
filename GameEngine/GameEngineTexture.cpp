#include "PreCompile.h"
#include "GameEngineTexture.h"

#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineString.h>

#pragma comment (lib, "DirectXTex.lib")

GameEngineTexture::GameEngineTexture() // default constructer ����Ʈ ������
	: Texture2D_(nullptr)
	, textureDesc_({0, })
	, RenderTargetView_(nullptr)
	, shaderResourceView_(nullptr)
{
}

GameEngineTexture::~GameEngineTexture() // default destructer ����Ʈ �Ҹ���
{
	if (nullptr != shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = nullptr;
	}

	if (nullptr != RenderTargetView_)
	{
		RenderTargetView_->Release();
		RenderTargetView_ = nullptr;
	}

	if (nullptr != Texture2D_)
	{
		Texture2D_->Release();
		Texture2D_ = nullptr;
	}
}

void GameEngineTexture::Create(ID3D11Texture2D* _Texture2D) 
{
	if (nullptr == _Texture2D)
	{
		GameEngineDebug::MsgBoxError("Texture Is null GameEngine Texture Create Error");
	}


	Texture2D_ = _Texture2D;
}

void GameEngineTexture::Load(const std::string& _path)
{
	GameEngineFile file(_path);
	std::string ext = file.GetExtension();

	GameEngineString::toupper(ext);

	if (ext == "TGA")
	{
		GameEngineDebug::MsgBoxError("���� �������� �ʴ� Ȯ���� �Դϴ�. " + ext);
	}
	else if (ext == "DDS")
	{
		GameEngineDebug::MsgBoxError("���� �������� �ʴ� Ȯ���� �Դϴ�. " + ext);
	}
	else
	{
		std::wstring path;
		GameEngineString::StringToWString(_path, path);
		if (S_OK != DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image_))
		{
			GameEngineDebug::MsgBoxError("WIC �̹��� �ε忡 �����߽��ϴ�. " + _path);
		}

		if (S_OK != DirectX::CreateShaderResourceView(GameEngineDevice::GetDevice()
			, image_.GetImages(), image_.GetImageCount(), image_.GetMetadata(), &shaderResourceView_))
		{
			GameEngineDebug::MsgBoxError("ShaderResourceView ���� ����. " + _path);
		}


		textureDesc_.Height = static_cast<UINT>(image_.GetImages()->height);
		textureDesc_.Width = static_cast<UINT>(image_.GetImages()->width);
		
	}


}


ID3D11RenderTargetView* GameEngineTexture::CreateRenderTargetView()
{
	if (nullptr != RenderTargetView_)
	{
		GameEngineDebug::MsgBoxError("RenderTargetView OverLap Create Error");
	}
	
	if (S_OK != GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D_, nullptr, &RenderTargetView_))
	{
		GameEngineDebug::MsgBoxError("RenderTargetView Create Error");
	}
	
	return RenderTargetView_;
}

ID3D11ShaderResourceView* GameEngineTexture::GetSRV()
{
	return shaderResourceView_;
}

float4 GameEngineTexture::GetSize()
{
	return float4(static_cast<float>(textureDesc_.Width), static_cast<float>(textureDesc_.Height));
}
