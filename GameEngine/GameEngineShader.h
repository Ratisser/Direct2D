#pragma once
#include <string>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"

#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

enum class ShaderType
{
	VS,
	PS,
	MAX
};

class GameEngineConstantBufferSetting;
class GameEngineSamplerSetting;
class GameEngineTextureSetting;
class GameEngineShader : public GameEngineObjectNameBase
{
public:
	GameEngineShader(ShaderType _Type);
	virtual ~GameEngineShader() = 0;

	GameEngineShader(const GameEngineShader& _Other) = delete;
	GameEngineShader(GameEngineShader&& _Other) noexcept = delete;
	GameEngineShader& operator=(const GameEngineShader& _Other) = delete;
	GameEngineShader& operator=(GameEngineShader&& _Other) noexcept = delete;

	std::map<unsigned int, GameEngineConstantBuffer*>& GetConstantBuffers();
	std::map<unsigned int, std::string>& GetTextures();
	std::map<unsigned int, GameEngineSampler*>& GetSamplers();

public:
	virtual void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void SetTexture(const GameEngineTextureSetting* _setting) = 0;
	virtual void SetSampler(const GameEngineSamplerSetting* _setting) = 0;

	virtual void ResetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void ResetTexture(const GameEngineTextureSetting* _Setting) = 0;
	virtual void ResetSampler(const GameEngineSamplerSetting* _Setting) = 0;

public:
	unsigned int GetTypeIndex();

	// 셰이더에 사용된 리소스를 체크하여 바인드 번호와 함께 맵에 저장하는 함수
	void ResCheck();

protected:
	void SetVersion(UINT _VersionHigh, UINT _VersionLow);
	void CreateVersion(const std::string& _ShaderType);
	void SetCode(const std::string& _Code);
	void SetEntryPoint(const std::string& _EntryPoint);

protected:
	UINT VersionHigh_;
	UINT VersionLow_;
	ID3DBlob* CodeBlob_;
	std::string Version_;
	std::string EntryPoint_;
	std::string Code_;
	ShaderType Type_;

private:
	std::map<unsigned int, GameEngineConstantBuffer*> ConstanceBuffer_;
	std::map<unsigned int, GameEngineSampler*> Samplers_;
	std::map<unsigned int, std::string> Textures_;
};

