#include "PreCompile.h"
#include "GameEngineTexture.h"

#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineString.h>
#include "GameEngineCore.h"

#pragma comment (lib, "DirectXTex.lib")

GameEngineTexture::GameEngineTexture() // default constructer 디폴트 생성자
	: Texture2D_(nullptr)
	, textureDesc_({ 0, })
	, RenderTargetView_(nullptr)
	, shaderResourceView_(nullptr)
{
}

GameEngineTexture::~GameEngineTexture() // default destructer 디폴트 소멸자
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
		return;
	}


	Texture2D_ = _Texture2D;
	Texture2D_->GetDesc(&textureDesc_);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = textureDesc_.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = 1;
	desc.Texture2D.MostDetailedMip = 0;

	GameEngineDevice::GetDevice()->CreateShaderResourceView(_Texture2D, &desc, &shaderResourceView_);
}

void GameEngineTexture::Load(const std::string& _path)
{
	GameEngineFile file(_path);
	std::string ext = file.GetExtension();

	GameEngineString::toupper(ext);

	if (ext == "TGA")
	{
		GameEngineDebug::MsgBoxError("현재 지원되지 않는 확장자 입니다. " + ext);
	}
	else if (ext == "DDS")
	{
		GameEngineDebug::MsgBoxError("현재 지원되지 않는 확장자 입니다. " + ext);
	}
	else
	{
		std::wstring path;
		GameEngineString::StringToWString(_path, path);

		if (S_OK != DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image_))
		{
			GameEngineDebug::MsgBoxError("WIC 이미지 로드에 실패했습니다. " + _path);
		}


		if (S_OK != DirectX::CreateShaderResourceView(GameEngineDevice::GetDevice()
			, image_.GetImages(), image_.GetImageCount(), image_.GetMetadata(), &shaderResourceView_))
		{
			GameEngineDebug::MsgBoxError("ShaderResourceView 생성 실패. " + _path);
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

ID3D11ShaderResourceView* GameEngineTexture::CreateShaderResourceView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = textureDesc_.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = 1;
	desc.Texture2D.MostDetailedMip = 0;

	GameEngineDevice::GetDevice()->CreateShaderResourceView(Texture2D_, &desc, &shaderResourceView_);
	return shaderResourceView_;
}

ID3D11ShaderResourceView* GameEngineTexture::GetShaderResourceView()
{
	return shaderResourceView_;
}

float4 GameEngineTexture::GetTextureSize()
{
	return float4(static_cast<float>(textureDesc_.Width), static_cast<float>(textureDesc_.Height));
}


void GameEngineTexture::PushCutIndex(const float4& _Size, const float4& _Pos)
{
	float4 CutUv;

	CutUv.x = _Pos.x;
	CutUv.y = _Pos.y;

	CutUv.z = _Size.x;
	CutUv.w = _Size.y;

	CutList_.push_back(CutUv);
}

void GameEngineTexture::Cut(int _x, int _y)
{
	for (int y = 0; y < _y; y++)
	{
		float4 CurStart;

		CurStart.z = 1.0f / _x;
		CurStart.w = 1.0f / _y;

		CurStart.y = CurStart.w * y;

		for (int x = 0; x < _x; x++)
		{
			CurStart.x = CurStart.z * x;
			CutList_.push_back(CurStart);
		}
	}

}


float4 GameEngineTexture::GetCutData(int _Index)
{
	if (0 == CutList_.size())
	{
		GameEngineDebug::MsgBoxError("자르지 않은 텍스처에서 인덱스를 얻어오려고 했습니다.");
	}

	if (_Index > CutList_.size())
	{
		GameEngineDebug::MsgBoxError("자른 개수에 비해서 인덱스가 너무 큽니다.");
	}

	return CutList_[_Index];
}

bool GameEngineTexture::IsCut()
{
	return CutList_.size() != 0;
}

float4 GameEngineTexture::GetPixel(int _x, int _y)
{
	if (0 > _x)
	{
		return float4::ZERO;
	}

	if (0 > _y)
	{
		return float4::ZERO;
	}

	if (image_.GetMetadata().width <= _x)
	{
		return float4::ZERO;
	}

	if (image_.GetMetadata().height <= _y)
	{
		return float4::ZERO;
	}

	DXGI_FORMAT Fmt = image_.GetMetadata().format;

	uint8_t* Color = image_.GetImages()->pixels;
	// int* ColorPtr = reinterpret_cast<int*>(Color);

	int Index = _y * static_cast<int>(image_.GetMetadata().width) + _x;
	Color = Color + (Index * 4);

	unsigned char B = Color[0];
	unsigned char G = Color[1];
	unsigned char R = Color[2];
	unsigned char A = Color[3];

	return float4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}