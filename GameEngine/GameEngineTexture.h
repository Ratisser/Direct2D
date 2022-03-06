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
	ID3D11ShaderResourceView* GetShaderResourceView();

	float4 GetTextureSize();

	bool IsCut();
	void Cut(int _x, int _y);
	void PushCutIndex(const float4& _Size, const float4& _Pos);
	float4 GetCutData(int _Index);


private:	// member Var
	ID3D11Texture2D* Texture2D_;
	D3D11_TEXTURE2D_DESC textureDesc_;
	ID3D11RenderTargetView* RenderTargetView_;
	ID3D11ShaderResourceView* shaderResourceView_;

	DirectX::ScratchImage image_;

	std::vector<float4> CutList_;
};

