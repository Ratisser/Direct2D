#pragma once

class GameEngineIndexBuffer : public GameEngineObjectNameBase
{
public:
	GameEngineIndexBuffer(); 
	~GameEngineIndexBuffer();

	GameEngineIndexBuffer(const GameEngineIndexBuffer& _other) = delete; 
	GameEngineIndexBuffer(GameEngineIndexBuffer&& _other) = delete; 

	GameEngineIndexBuffer& operator=(const GameEngineIndexBuffer& _other) = delete;
	GameEngineIndexBuffer& operator=(const GameEngineIndexBuffer&& _other) = delete;

	void Create(const std::vector<int>& _indices);
	const std::vector<int>& GetIndices();

private:
	std::vector<int> indices_;

};

