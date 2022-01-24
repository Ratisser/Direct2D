#pragma once
#include <GameEngineBase/GameEngineMath.h>

// �з� : 
// �뵵 : 
// ���� : 
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
	GameEngineRenderingPipeLine(); // default constructer ����Ʈ ������
	~GameEngineRenderingPipeLine(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
};
