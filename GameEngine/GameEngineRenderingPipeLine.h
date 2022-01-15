#pragma once
#include <GameEngineBase/GameEngineMath.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEngineRenderingPipeLine
{
private:	// member Var
	GameEngineVertexBuffer* VertexBuffer_;
	GameEngineVertexShader* VertexShader_;

	GameEngineIndexBuffer* IndexBuffer_;

public:
	void SetInputAssembler1(const std::string& _Name);

	void SetVertexShader(const std::string& _Name);

	void SetInputAssembler2(const std::string& _Name);

public:
	void Rendering();


public:
	GameEngineRenderingPipeLine(); // default constructer 디폴트 생성자
	~GameEngineRenderingPipeLine(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
};

