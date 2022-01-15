#pragma once

class GameEngineVertexShader : public GameEngineObjectNameBase
{
public:
	GameEngineVertexShader(); 
	~GameEngineVertexShader();

	GameEngineVertexShader(const GameEngineVertexShader& _other) = delete; 
	GameEngineVertexShader(GameEngineVertexShader&& _other) = delete; 

	GameEngineVertexShader& operator=(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader& operator=(const GameEngineVertexShader&& _other) = delete;

	const std::function<float4(const float4&)> GetShaderFunction();
	void Create(std::function<float4(const float4)> _function);


private:
	std::function<float4(const float4& _vertex)> shaderFunction_;
};

