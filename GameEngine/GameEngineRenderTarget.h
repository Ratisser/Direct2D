#pragma once
#include "GameEngineDevice.h"

class GameEngineTexture;
class GameEngineRenderTarget : public GameEngineObjectNameBase
{
private:	// member Var
	// 텍스처에게서 빌려오는 겁니다.
	std::vector<GameEngineTexture*> Textures_;
	// std::vector<D3D11_VIEWPORT> ViewPort_;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews_;

	std::vector<float4> ClearColor_;

public:
	void Clear();

	// -1이면 전부다 세팅하라는 의미가 됩니다.
	// 0~n은 그 인덱스 1개를 세팅하라는 의미가 됩니다.
	void Setting(int _Index = -1);

public:
	void Create(const std::string _TextureName, float4 _ClearColor);

public:
	GameEngineRenderTarget(); // default constructer 디폴트 생성자
	~GameEngineRenderTarget(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

};

