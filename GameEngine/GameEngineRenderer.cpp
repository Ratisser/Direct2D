#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineRenderingPipelineManager.h"
#include "GameEngineRenderingPipeline.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"

GameEngineRenderer::GameEngineRenderer()
	: pipe_(nullptr)
{

}

GameEngineRenderer::~GameEngineRenderer()
{

}

void GameEngineRenderer::SetRenderingPipeline(const std::string& _name)
{
	pipe_ = GameEngineRenderingPipelineManager::GetInst().Find(_name);

	if (nullptr == pipe_)
	{
		GameEngineDebug::MsgBoxError("no pipeline named \"" + _name + "\"");
		return;
	}

	ShaderHelper_.ShaderResourcesCheck(pipe_->GetVertexShader());
	ShaderHelper_.ShaderResourcesCheck(pipe_->GetPixelShader());

	if (true == ShaderHelper_.IsValidConstantBuffer("TransformData"))
	{
		ShaderHelper_.SettingConstantBufferLink("TransformData", GetTransformData());
	}

	if (nullptr == pipe_)
	{
		GameEngineDebug::MsgBoxError("invalid pipeline name  \"" + _name + "\"");
	}
}

void GameEngineRenderer::SetTextureByValue(const std::string& _valueName, const std::string& _textureName, bool bSrcScale)
{
	GameEngineTexture* tex = ShaderHelper_.SettingTexture(_valueName, _textureName);
	if (bSrcScale)
	{
		float4 size = tex->GetTextureSize();
		SetScale(size.x, size.y);
	}
}

void GameEngineRenderer::SetTexture(const std::string& _textureName, bool bSrcScale)
{
	GameEngineTexture* tex = ShaderHelper_.SettingTexture("Tex", _textureName);
	if (bSrcScale)
	{
		float4 size = tex->GetTextureSize();
		SetScale(size.x, size.y);
	}
}

void GameEngineRenderer::SetBlender(const std::string& _blenderName)
{
	pipe_->SetOutputMergerBlend(_blenderName);
}

void GameEngineRenderer::Render()
{
	ShaderHelper_.Setting();
	pipe_->Rendering();
}

void GameEngineRenderer::Start()
{
	actor_->GetLevel()->pushRenderer(this);
}

void GameEngineRenderer::Update(float _deltaTime)
{
}
