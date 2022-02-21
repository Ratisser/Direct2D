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

	if (true == pipe_->ShaderHelper.IsValidConstantBuffer("TransformData"))
	{
		pipe_->ShaderHelper.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
	}

	if (nullptr == pipe_)
	{
		GameEngineDebug::MsgBoxError("invalid pipeline name  \"" + _name + "\"");
	}
}

void GameEngineRenderingComponent::Render()
{
	pipe_->Rendering();
}

void GameEngineRenderingComponent::Start()
{
}

void GameEngineRenderingComponent::Update()
{
}
