#pragma once
#include <GameEngine/GameEngineDevice.h>
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineShaderResHelper.h"



// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineShader;
class GameEngineConstantBuffer;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEnginePixelShader;
class GameEngineRasterizer;
class GameEngineBlender;
class GameEngineRenderingPipeline : public GameEngineObjectNameBase
{
private:	// member Var
	// IA1
	GameEngineVertexBuffer* vertexBuffer_;
	// IA1
	GameEngineVertexShader* inputLayOutVertexShader_;
	// VS
	GameEngineVertexShader* vertexShader_;
	// IA2
	GameEngineIndexBuffer* indexBuffer_;
	// IA2
	D3D11_PRIMITIVE_TOPOLOGY topology_;
	// RS
	GameEngineRasterizer* rasterizer_;
	// PS
	GameEnginePixelShader* pixelShader_;
	// OM
	GameEngineRenderTarget* renderTarget_;
	GameEngineBlender* blender_;

public:
	void SetInputAssembler1VertexBufferSetting(const std::string& _Name);

	void SetInputAssembler1InputLayOutSetting(const std::string& _Name);

	void SetVertexShader(const std::string& _Name);

	void SetInputAssembler2IndexBufferSetting(const std::string& _Name);

	void SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY Topology_);

	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	GameEngineVertexShader* GetVertexShader() const;
	GameEnginePixelShader* GetPixelShader() const;

	void RenderingPipeLineSetting();
	void Reset();

	void Rendering();

public:
	GameEngineRenderingPipeline(); // default constructer 디폴트 생성자
	~GameEngineRenderingPipeline(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRenderingPipeline(const GameEngineRenderingPipeline& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRenderingPipeline(GameEngineRenderingPipeline&& _other) = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRenderingPipeline& operator=(const GameEngineRenderingPipeline& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRenderingPipeline& operator=(const GameEngineRenderingPipeline&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

	void InputAssembler1();
	void InputAssembler2();
	void VertexShader();
	void Rasterizer();
	void PixelShader();
	void OutputMerger();
};

