#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineMesh : public GameEngineObjectNameBase
{
private:	// member Var
	GameEngineVertexBuffer* VertexBuffer_;
	GameEngineIndexBuffer* IndexBuffer_;

public:
	GameEngineMesh(); // default constructer 디폴트 생성자
	~GameEngineMesh(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineMesh(const GameEngineMesh& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineMesh(GameEngineMesh&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineMesh& operator=(const GameEngineMesh& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineMesh& operator=(const GameEngineMesh&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
};

