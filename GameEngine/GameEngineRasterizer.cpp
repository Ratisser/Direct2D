#include "PreCompile.h"
#include "GameEngineRasterizer.h"

GameEngineRasterizer::GameEngineRasterizer()
{

}

GameEngineRasterizer::~GameEngineRasterizer()
{

}

void GameEngineRasterizer::SetViewPort(float _screenX, float _screenY, float _startX, float _startY, float _minZ, float _maxZ)
{
	viewport_.ViewPortCenter(_screenX, _screenY, _startX, _startY, _minZ, _maxZ);
}

void GameEngineRasterizer::RasterizerUpdate(float4& _pos)
{
	_pos.x /= _pos.w;
	_pos.y /= _pos.w;
	_pos.z /= _pos.w;
	_pos.w = 1.f;

	_pos *= viewport_;
}
