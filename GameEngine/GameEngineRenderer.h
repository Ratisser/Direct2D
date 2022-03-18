#pragma once

#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"

class GameEngineRenderingPipeline;
class GameEngineRenderer : public GameEngineTransformComponent
{
public:
	GameEngineRenderer(); 
	~GameEngineRenderer();

	GameEngineRenderer(const GameEngineRenderer& _other) = delete; 
	GameEngineRenderer(GameEngineRenderer&& _other) = delete; 

	GameEngineRenderer& operator=(const GameEngineRenderer& _other) = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer&& _other) = delete;

	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

	void SetRenderingPipeline(const std::string& _name);
	void SetTextureByValue(const std::string& _valueName, const std::string& _textureName, bool bSrcScale);
	void SetTexture(const std::string& _textureName, bool bSrcScale);
	void SetBlender(const std::string& _blenderName);
	void Render();

protected:
	GameEngineRenderingPipeline* pipe_;
	GameEngineShaderResHelper ShaderHelper_;
};

