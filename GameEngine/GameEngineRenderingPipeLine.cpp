#include "PreCompile.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"


#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineWindow.h"

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine() // default constructer 디폴트 생성자
	: VertexBuffer_(nullptr)
	, VertexShader_(nullptr)
	, IndexBuffer_(nullptr)
{

}

GameEngineRenderingPipeLine::~GameEngineRenderingPipeLine() // default destructer 디폴트 소멸자
{

}

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
	: VertexBuffer_(_other.VertexBuffer_)
	, VertexShader_(_other.VertexShader_)
	, IndexBuffer_(_other.IndexBuffer_)
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


void GameEngineRenderingPipeLine::Rendering()
{
	// input어셈블러 단계
	std::vector<float4> CopyVertex;
	{
		CopyVertex = VertexBuffer_->GetVertices();
	}

	{
		for (size_t i = 0; i < CopyVertex.size(); i++)
		{
			CopyVertex[i] = VertexShader_->GetShaderFunction()((CopyVertex[i]));
		}
	}

	// 그린다.
	{
		const std::vector<int>& Index = IndexBuffer_->GetIndices();

		POINT ArrTri[3];

		for (size_t TriCount = 0; TriCount < Index.size() / 3; TriCount++)
		{
			for (size_t i = 0; i < 3; i++)
			{
				int CurIndex = Index[(TriCount * 3) + i];

				ArrTri[i] = CopyVertex[CurIndex].GetWindowPoint();
			}

			// 이게 픽셀 쉐이더 단계라고 볼수 있다.
			Polygon(GameEngineWindow::GetInst().GetWindowDC(), &ArrTri[0], 3);
		}
	}
}