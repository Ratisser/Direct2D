#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"

class GameEngineTexture;
class GameEngineRenderTarget : public GameEngineObjectNameBase
{
public:
	GameEngineRenderTarget(); // default constructer 디폴트 생성자
	~GameEngineRenderTarget(); // default destructer 디폴트 소멸자
	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete;

public:
	void Create(const std::string _TextureName, float4 _ClearColor);
	void Create(GameEngineTexture* _texture, float4 _ClearColor);
	void Clear();

	void Setting(int _Index = -1);

	float4 GetTextureSize(size_t _Index);
	GameEngineTexture* GetTexture(size_t _Index);
	ID3D11RenderTargetView* GetRenderTargetView(size_t _index);

private:
	std::vector<GameEngineTexture*> Textures_;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews_;
	std::vector<float4> ClearColor_;
};

