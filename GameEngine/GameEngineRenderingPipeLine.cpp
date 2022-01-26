#include "PreCompile.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"
#include "GameEngineRasterizerManager.h"


#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineWindow.h"
#include "GameEngineRasterizer.h"

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine() // default constructer 디폴트 생성자
	: VertexBuffer_(nullptr)
	, VertexShader_(nullptr)
	, IndexBuffer_(nullptr)
	, Rasterizer_(nullptr)
{

}

GameEngineRenderingPipeLine::~GameEngineRenderingPipeLine() // default destructer 디폴트 소멸자
{

}

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
	: VertexBuffer_(_other.VertexBuffer_)
	, VertexShader_(_other.VertexShader_)
	, IndexBuffer_(_other.IndexBuffer_)
	, Rasterizer_(_other.Rasterizer_)
{

}

void GameEngineRenderingPipeLine::SetInputAssembler1(const std::string& _Name)
{
	VertexBuffer_ = GameEngineVertexBufferManager::GetInst().Find(_Name);

	if (nullptr == VertexBuffer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}

}

void GameEngineRenderingPipeLine::SetVertexShader(const std::string& _Name)
{
	VertexShader_ = GameEngineVertexShaderManager::GetInst().Find(_Name);

	if (nullptr == VertexShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetInputAssembler2(const std::string& _Name)
{
	IndexBuffer_ = GameEngineIndexBufferManager::GetInst().Find(_Name);

	if (nullptr == VertexShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetRasterizer(const std::string& _Name)
{
	Rasterizer_ = GameEngineRasterizerManager::GetInst().Find(_Name);

	if (nullptr == Rasterizer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 레이터라이저 세팅을 세팅하려고 했습니다.");
		return;
	}
}


void GameEngineRenderingPipeLine::Rendering()
{
	// Input Assembler 1
	std::vector<float4> CopyVertex;
	CopyVertex = VertexBuffer_->GetVertices();

	// Vertex Shader
	for (size_t i = 0; i < CopyVertex.size(); i++)
	{
		CopyVertex[i] = VertexShader_->GetShaderFunction()((CopyVertex[i]));
	}

	// Input Assembler 2
	const std::vector<int>& Index = IndexBuffer_->GetIndices();


	std::vector<std::vector<float4>> TriVector;
	// 그린다.
	{
		const std::vector<int>& Index = IndexBuffer_->GetIndices();


		TriVector.resize(Index.size() / 3);


		for (size_t TriCount = 0; TriCount < Index.size() / 3; TriCount++)
		{
			TriVector[TriCount].resize(3);

			int CurIndex0 = Index[(TriCount * 3) + 0];
			int CurIndex1 = Index[(TriCount * 3) + 1];
			int CurIndex2 = Index[(TriCount * 3) + 2];

			TriVector[TriCount][0] = CopyVertex[CurIndex0];
			TriVector[TriCount][1] = CopyVertex[CurIndex1];
			TriVector[TriCount][2] = CopyVertex[CurIndex2];
		}
	}

	for (size_t Tri = 0; Tri < TriVector.size(); Tri++)
	{
		for (size_t i = 0; i < TriVector[Tri].size(); i++)
		{
			Rasterizer_->RasterizerUpdate(TriVector[Tri][i]);
		}
	}

	for (size_t Tri = 0; Tri < TriVector.size(); Tri++)
	{

		POINT ArrTri[3];

		ArrTri[0] = TriVector[Tri][0].GetWindowPoint();
		ArrTri[1] = TriVector[Tri][1].GetWindowPoint();
		ArrTri[2] = TriVector[Tri][2].GetWindowPoint();

		Polygon(GameEngineWindow::GetInst().GetWindowDC(), &ArrTri[0], 3);
	}

}