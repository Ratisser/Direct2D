#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

class GameEngineTexture : public GameEngineObjectNameBase
{
public:
	GameEngineTexture();
	~GameEngineTexture(); 
	GameEngineTexture(const GameEngineTexture& _other) = delete;
	GameEngineTexture(GameEngineTexture&& _other) noexcept = delete; 
	GameEngineTexture& operator=(const GameEngineTexture& _other) = delete; 
	GameEngineTexture& operator=(const GameEngineTexture&& _other) = delete; 

public:
	void Create(ID3D11Texture2D* _Texture2D);
	void Load(const std::string& _path);
	
	ID3D11RenderTargetView* CreateRenderTargetView();
	ID3D11ShaderResourceView* GetSRV();

private:	// member Var
	ID3D11Texture2D* Texture2D_;
	D3D11_TEXTURE2D_DESC textureDesc_;
	ID3D11RenderTargetView* RenderTargetView_;
	ID3D11ShaderResourceView* shaderResourceView_;

	DirectX::ScratchImage image_;
};

