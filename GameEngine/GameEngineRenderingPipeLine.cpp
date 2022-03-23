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

GameEngineRenderingPipeline::GameEngineRenderingPipeline() // default constructer 디폴트 생성자
	: VertexBuffer_(nullptr)
	, InputLayOutVertexShader_(nullptr)
	, VertexShader_(nullptr)
	, IndexBuffer_(nullptr)
	, PixelShader_(nullptr)
	, Rasterizer_(nullptr)
	, RenderTarget_(nullptr)
	, Blender_(nullptr)
	, Topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

GameEngineRenderingPipeline::~GameEngineRenderingPipeline() // default destructer 디폴트 소멸자
{

}

void GameEngineRenderingPipeline::SetInputAssembler1VertexBufferSetting(const std::string& _Name)
{
	VertexBuffer_ = GameEngineVertexBufferManager::GetInst().Find(_Name);

	if (nullptr == VertexBuffer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}

}

void GameEngineRenderingPipeline::SetInputAssembler1InputLayOutSetting(const std::string& _Name) 
{
	InputLayOutVertexShader_ = GameEngineVertexShaderManager::GetInst().Find(_Name);

	if (nullptr == InputLayOutVertexShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeline::SetInputAssembler2IndexBufferSetting(const std::string& _Name) 
{
	IndexBuffer_ = GameEngineIndexBufferManager::GetInst().Find(_Name);

	if (nullptr == IndexBuffer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 버퍼를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeline::SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY _Topology) 
{
	Topology_ = _Topology;
}


void GameEngineRenderingPipeline::SetVertexShader(const std::string& _Name)
{
	VertexShader_ = GameEngineVertexShaderManager::GetInst().Find(_Name);

	if (nullptr == VertexShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
		return;
	}


}

void GameEngineRenderingPipeline::SetRasterizer(const std::string& _Name) 
{
	Rasterizer_ = GameEngineRasterizerManager::GetInst().Find(_Name);

	if (nullptr == Rasterizer_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 레이터라이저 세팅을 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeline::SetPixelShader(const std::string& _Name) 
{
	PixelShader_ = GameEnginePixelShaderManager::GetInst().Find(_Name);

	if (nullptr == PixelShader_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 픽셀 쉐이더를 세팅을 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeline::SetOutputMergerBlend(const std::string& _Name) 
{
	Blender_ = GameEngineBlenderManager::GetInst().Find(_Name);

	if (nullptr == Blender_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 블렌드를 세팅을 세팅하려고 했습니다.");
		return;
	}
}

GameEngineVertexShader* GameEngineRenderingPipeline::GetVertexShader() const
{
	return VertexShader_;
}

GameEnginePixelShader* GameEngineRenderingPipeline::GetPixelShader() const
{
	return PixelShader_;
}

void GameEngineRenderingPipeline::InputAssembler1() 
{
	VertexBuffer_->Setting();
	InputLayOutVertexShader_->InputLayOutSetting();
}

void GameEngineRenderingPipeline::InputAssembler2() 
{
	IndexBuffer_->Setting();
	GameEngineDevice::GetContext()->IASetPrimitiveTopology(Topology_);
}

void GameEngineRenderingPipeline::VertexShader() 
{
	VertexShader_->Setting();
}

void GameEngineRenderingPipeline::Rasterizer()
{
	Rasterizer_->Setting();
	Rasterizer_->SettingViewPort();
}


void GameEngineRenderingPipeline::PixelShader()
{
	PixelShader_->Setting();
}

void GameEngineRenderingPipeline::OutputMerger()
{
	Blender_->Setting();
}

void GameEngineRenderingPipeline::RenderingPipeLineSetting()
{
	// input어셈블러 단계
	InputAssembler1();

	InputAssembler2();

	VertexShader();

	Rasterizer();

	PixelShader();

	OutputMerger();
}

void GameEngineRenderingPipeline::Rendering() 
{
	RenderingPipeLineSetting();

	GameEngineDevice::GetContext()->DrawIndexed(IndexBuffer_->GetIndexCount(), 0, 0);
}



