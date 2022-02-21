#include "PreCompile.h"
#include "GameEngineRenderingComponent.h"
#include "GameEngineRenderingPipelineManager.h"
#include "GameEngineRenderingPipeline.h"

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

	pipe_ = GameEngineRenderingPipelineManager::GetInst().Find("Color");

	if (true == pipe_->ShaderHelper.IsConstantBuffer("TransformData"))
	{
		pipe_->ShaderHelper.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
	}

	if (nullptr == pipe_)
	{
		GameEngineDebug::MsgBoxError("unavailable pipeline named \"" + _name + "\"");
	}
}
