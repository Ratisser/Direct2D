#pragma once
#include <vector>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineShader.h"
#include "GameEngineDevice.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineVertexShader : public GameEngineShader
{
public:
	GameEngineVertexShader();
	~GameEngineVertexShader();
	GameEngineVertexShader(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader(GameEngineVertexShader&& _other) noexcept = delete;
	GameEngineVertexShader& operator=(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader& operator=(const GameEngineVertexShader&& _other) = delete;

public:
	virtual void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	virtual void SetTexture(const GameEngineTextureSetting* _setting) override;
	virtual void SetSampler(const GameEngineSamplerSetting* _setting) override;

public:
	bool Create(const std::string& _ShaderCode, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	bool Load(const std::string& _Path,const std::string& _EntryPoint,UINT _VersionHigh = 5,UINT _VersionLow = 0);

	void InputLayOutSetting();
	void Setting();
	bool StringCompile();
	bool FileCompile(const std::string& _Path);


private:
	void AddInputLayOut(const char* _SemanticName, unsigned int _Index,	unsigned int _AlignedByteOffset, DXGI_FORMAT _Format,
		unsigned int _InputSlot, unsigned int _InstanceDataStepRate, D3D11_INPUT_CLASSIFICATION _inputClass);
	void CreateLayOut();
	void LayOutCheck();
	void LayOutClear();

private:
	ID3D11VertexShader* Shader_;

	ID3D11InputLayout* LayOut_;
	unsigned int LayOutOffset_;
	std::vector<std::string> SemanticName_;
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc_;
};

