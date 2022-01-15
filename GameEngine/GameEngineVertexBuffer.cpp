#include "PreCompile.h"
#include "GameEngineVertexBuffer.h"

GameEngineVertexBuffer::GameEngineVertexBuffer()
{

}

GameEngineVertexBuffer::~GameEngineVertexBuffer()
{

}

void GameEngineVertexBuffer::Create(const std::vector<float4>& _vertices)
{
	vertices_ = _vertices;
}

const std::vector<float4>& GameEngineVertexBuffer::GetVertices()
{
	return vertices_;
}
