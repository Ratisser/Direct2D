#include "PreCompile.h"
#include "GameEngineRenderingPipeline.h"
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"
#include "GameEngineRasterizerManager.h"
#include "GameEnginePixelShaderManager.h"
#include <GameEngine/GameEngineBlenderManager.h>

#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineBlender.h"


#include "GameEngineWindow.h"

GameEngineRenderingPipeline::GameEngineRenderingPipeline() // default constructer ����Ʈ ������
	: vertexBuffer_(nullptr)
	, inputLayOutVertexShader_(nullptr)
	, vertexShader_(nullptr)
	, indexBuffer_(nullptr)
	, pixelShader_(nullptr)
	, rasterizer_(nullptr)
	, renderTarget_(nullptr)
	, blender_(nullptr)
	, topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

GameEngineRenderingPipeline::~GameEngineRenderingPipeline() // default destructer ����Ʈ �Ҹ���
{

}

void GameEngineRenderingPipeline::SetInputAssembler1VertexBufferSetting(const std::string& _Name)
{
	vertexBuffer_ = GameEngineVertexBufferManager::GetInst().Find(_Name);

	if (nullptr == vertexBuffer_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���ؽ� ���۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}

}

void GameEngineRenderingPipeline::SetInputAssembler1InputLayOutSetting(const std::string& _Name) 
{
	inputLayOutVertexShader_ = GameEngineVertexShaderManager::GetInst().Find(_Name);

	if (nullptr == inputLayOutVertexShader_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���ؽ� ���۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeline::SetInputAssembler2IndexBufferSetting(const std::string& _Name) 
{
	indexBuffer_ = GameEngineIndexBufferManager::GetInst().Find(_Name);

	if (nullptr == indexBuffer_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���ؽ� ���۸� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeline::SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY _Topology) 
{
	topology_ = _Topology;
}


void GameEngineRenderingPipeline::SetVertexShader(const std::string& _Name)
{
	vertexShader_ = GameEngineVertexShaderManager::GetInst().Find(_Name);

	if (nullptr == vertexShader_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���ؽ� ���̴��� �����Ϸ��� �߽��ϴ�.");
		return;
	}


}

void GameEngineRenderingPipeline::SetRasterizer(const std::string& _Name) 
{
	rasterizer_ = GameEngineRasterizerManager::GetInst().Find(_Name);

	if (nullptr == rasterizer_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �����Ͷ����� ������ �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeline::SetPixelShader(const std::string& _Name) 
{
	pixelShader_ = GameEnginePixelShaderManager::GetInst().Find(_Name);

	if (nullptr == pixelShader_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ȼ� ���̴��� ������ �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeline::SetOutputMergerBlend(const std::string& _Name) 
{
	blender_ = GameEngineBlenderManager::GetInst().Find(_Name);

	if (nullptr == blender_)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���带 ������ �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

GameEngineVertexShader* GameEngineRenderingPipeline::GetVertexShader() const
{
	return vertexShader_;
}

GameEnginePixelShader* GameEngineRenderingPipeline::GetPixelShader() const
{
	return pixelShader_;
}

void GameEngineRenderingPipeline::InputAssembler1() 
{
	vertexBuffer_->Setting();
	inputLayOutVertexShader_->InputLayOutSetting();
}

void GameEngineRenderingPipeline::InputAssembler2() 
{
	indexBuffer_->Setting();
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(topology_);
}

void GameEngineRenderingPipeline::VertexShader() 
{
	vertexShader_->Setting();
}

void GameEngineRenderingPipeline::Rasterizer()
{
	rasterizer_->Setting();
	rasterizer_->SettingViewPort();
}


void GameEngineRenderingPipeline::PixelShader()
{
	pixelShader_->Setting();
}

void GameEngineRenderingPipeline::OutputMerger()
{
	blender_->Setting();
}

void GameEngineRenderingPipeline::RenderingPipeLineSetting()
{
	// input����� �ܰ�
	InputAssembler1();

	InputAssembler2();

	VertexShader();

	Rasterizer();

	PixelShader();

	OutputMerger();
}

void GameEngineRenderingPipeline::Reset()
{
	blender_->Reset();
	GameEngineDevice::GetContext()->OMSetDepthStencilState(nullptr, 0);
}

void GameEngineRenderingPipeline::Rendering() 
{
	RenderingPipeLineSetting();

	GameEngineDevice::GetContext()->DrawIndexed(indexBuffer_->GetIndexCount(), 0, 0);
}



