#include "PreCompile.h"
#include "GameEngineIndexBuffer.h"

GameEngineIndexBuffer::GameEngineIndexBuffer()
{

}

GameEngineIndexBuffer::~GameEngineIndexBuffer()
{

}

void GameEngineIndexBuffer::Create(const std::vector<int>& _indices)
{
	indices_ = _indices;
}

const std::vector<int>& GameEngineIndexBuffer::GetIndices()
{
	return indices_;
}
