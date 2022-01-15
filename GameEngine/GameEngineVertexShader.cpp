#include "PreCompile.h"
#include "GameEngineVertexShader.h"

GameEngineVertexShader::GameEngineVertexShader()
{

}

GameEngineVertexShader::~GameEngineVertexShader()
{

}

const std::function<float4(const float4&)> GameEngineVertexShader::GetShaderFunction()
{
	return shaderFunction_;
}

void GameEngineVertexShader::Create(std::function<float4(const float4)> _function)
{
	shaderFunction_ = _function;
}
