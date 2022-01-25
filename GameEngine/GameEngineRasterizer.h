#pragma once

class GameEngineRasterizer : public GameEngineObjectNameBase
{
public:
	GameEngineRasterizer(); 
	~GameEngineRasterizer();

	GameEngineRasterizer(const GameEngineRasterizer& _other) = delete; 
	GameEngineRasterizer(GameEngineRasterizer&& _other) = delete; 

	GameEngineRasterizer& operator=(const GameEngineRasterizer& _other) = delete;
	GameEngineRasterizer& operator=(const GameEngineRasterizer&& _other) = delete;

public:
	void SetViewPort(float _screenX, float _screenY, float _startX, float _startY, float _minZ, float _maxZ);

	void RasterizerUpdate(float4& _pos);

private:
	float4x4 viewport_;
};

