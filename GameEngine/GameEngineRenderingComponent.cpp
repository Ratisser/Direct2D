#include "PreCompile.h"
#include "GameEngineRenderingComponent.h"
#include "GameEngineRenderingPipelineManager.h"
#include "GameEngineRenderingPipeline.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

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
	actor_->GetLevel()->pushRenderingComponent(this);
}

void GameEngineRenderingComponent::Update()
{
}
