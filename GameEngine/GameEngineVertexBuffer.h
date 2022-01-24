#pragma once

class GameEngineVertexBuffer : public GameEngineObjectNameBase
{
public:
	GameEngineVertexBuffer(); 
	~GameEngineVertexBuffer();

	GameEngineVertexBuffer(const GameEngineVertexBuffer& _other) = delete; 
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _other) = delete; 

	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer&& _other) = delete;


	void						Create(const std::vector<float4>& _vertices);
	const std::vector<float4>&	GetVertices();

private:
	std::vector<float4> vertices_;
};

