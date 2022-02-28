#include "PreCompile.h"
#include "GameEngineRenderingComponent.h"
#include "GameEngineRenderingPipelineManager.h"
#include "GameEngineRenderingPipeline.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"

GameEngineRenderingComponent::GameEngineRenderingComponent()
	: pipe_(nullptr)
{

}

GameEngineRenderingComponent::~GameEngineRenderingComponent()
{

}

void GameEngineRenderingComponent::SetRenderingPipeline(const std::string& _name)
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
		ShaderHelper_.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
	}

	if (nullptr == pipe_)
	{
		GameEngineDebug::MsgBoxError("invalid pipeline name  \"" + _name + "\"");
	}
}

void GameEngineRenderingComponent::SetTextureByValue(const std::string& _valueName, const std::string& _textureName, bool bSrcScale)
{
	GameEngineTexture* tex = ShaderHelper_.SettingTexture(_valueName, _textureName);
	if (bSrcScale)
	{
		float4 size = tex->GetSize();
		transform_->SetScale(size.x, size.y);
	}
}

void GameEngineRenderingComponent::SetTexture(const std::string& _textureName, bool bSrcScale)
{
	GameEngineTexture* tex = ShaderHelper_.SettingTexture("Tex", _textureName);
	if (bSrcScale)
	{
		float4 size = tex->GetSize();
		transform_->SetScale(size.x, size.y);
	}
}

void GameEngineRenderingComponent::SetBlender(const std::string& _blenderName)
{
	pipe_->SetOutputMergerBlend(_blenderName);
}

void GameEngineRenderingComponent::Render()
{
	ShaderHelper_.Setting();
	pipe_->Rendering();
}

void GameEngineRenderingComponent::Start()
{
	actor_->GetLevel()->pushRenderingComponent(this);
}

void GameEngineRenderingComponent::Update()
{
}
