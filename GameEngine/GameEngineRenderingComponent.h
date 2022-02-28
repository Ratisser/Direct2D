#pragma once

#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"

class GameEngineRenderingPipeline;
class GameEngineRenderingComponent : public GameEngineTransformComponent
{
public:
	GameEngineRenderingComponent(); 
	~GameEngineRenderingComponent();

	GameEngineRenderingComponent(const GameEngineRenderingComponent& _other) = delete; 
	GameEngineRenderingComponent(GameEngineRenderingComponent&& _other) = delete; 

	GameEngineRenderingComponent& operator=(const GameEngineRenderingComponent& _other) = delete;
	GameEngineRenderingComponent& operator=(const GameEngineRenderingComponent&& _other) = delete;

	virtual void Start() override;
	virtual void Update() override;

	void SetRenderingPipeline(const std::string& _name);
	void SetTextureByValue(const std::string& _valueName, const std::string& _textureName, bool bSrcScale);
	void SetTexture(const std::string& _textureName, bool bSrcScale);
	void SetBlender(const std::string& _blenderName);
	void Render();

protected:
	GameEngineRenderingPipeline* pipe_;
	GameEngineShaderResHelper ShaderHelper_;

};

