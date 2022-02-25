#pragma once
#include <string>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "GameEngine/GameEngineConstantBuffer.h"
#include "GameEngine/GameEngineSampler.h"

enum class ShaderType
{
	VS,
	PS,
	MAX
};

// Ό³Έν :
class GameEngineConstantBufferSetting;
class GameEngineTextureSetting;
class GameEngineShader : public GameEngineObjectNameBase 
{
public:
	// constrcuter destructer
	GameEngineShader(ShaderType _Type);
	virtual ~GameEngineShader() = 0;

	// delete Function
	GameEngineShader(const GameEngineShader& _Other) = delete;
	GameEngineShader(GameEngineShader&& _Other) noexcept = delete;
	GameEngineShader& operator=(const GameEngineShader& _Other) = delete;
	GameEngineShader& operator=(GameEngineShader&& _Other) noexcept = delete;

	

protected:
	UINT VersionHigh_;
	UINT VersionLow_;
	ID3DBlob* CodeBlob_;
	std::string Version_;
	std::string EntryPoint_;
	std::string Code_;
	ShaderType Type_;


	void SetVersion(UINT _VersionHigh, UINT _VersionLow);
	void CreateVersion(const std::string& _ShaderType);
	void SetCode(const std::string& _Code);
	void SetEntryPoint(const std::string& _EntryPoint);

public:
	unsigned int GetTypeIndex()
	{
		return static_cast<unsigned int>(Type_);
	}

	void ResCheck();

private:
	std::map<unsigned int, GameEngineConstantBuffer*> ConstanceBuffer_;
	std::map<unsigned int, GameEngineSampler*> Samplers_;
	std::map<unsigned int, std::string> Textures_;

public:
	std::map<unsigned int, GameEngineConstantBuffer*>& GetConstantBuffers() 
	{
		return ConstanceBuffer_;
	}

	virtual void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void SetTexture(const GameEngineTextureSetting* _setting) = 0;

};

