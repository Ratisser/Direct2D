#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"

class GameEngineBlender: public GameEngineObjectNameBase
{
public:
	GameEngineBlender();
	~GameEngineBlender();

	GameEngineBlender(const GameEngineBlender& _other) = delete;
	GameEngineBlender(GameEngineBlender&& _other) = delete;

	GameEngineBlender& operator=(const GameEngineBlender& _other) = delete;
	GameEngineBlender& operator=(const GameEngineBlender&& _other) = delete;
public:
	void Create(const D3D11_BLEND_DESC& _desc);
	void Setting();
	void Reset();

private:
	D3D11_BLEND_DESC desc_;
	ID3D11BlendState* state_;
};

