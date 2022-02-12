#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>

UserGame::UserGame() // default constructer 디폴트 생성자
{

}

UserGame::~UserGame() // default destructer 디폴트 소멸자
{

}

UserGame::UserGame(UserGame&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



struct TransformData
{
	float4x4 World;
	float4x4 View;
	float4x4 Proj;
};

float4 Pos;
TransformData TransData;

void UserGame::Initialize()
{
	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("ColorRendering");
	// cpu의 데이터와 상수버퍼를 연결한다.
	Pipe->ShaderHelper.SettingConstantBufferLink("TransformData", TransData);


	// Pipe->ShaderHelper.SettingConstantBufferLink("TransformData222", TransData);
	// cpu의 데이터와 상수버퍼를 한번 복사한다.
	// Pipe->ShaderHelper.SettingConstantBufferSet("TransformData", TransData);

	return;
}

void UserGame::Release()
{

}



void UserGame::GameLoop()
{
	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("ColorRendering");
	{
		Pos.x += 0.1f * GameEngineTime::GetInst().GetDeltaTime();
		TransData.World.Translation(Pos);


		// 랜더링 파이프라인이 하나가 돌고
		// 이안에 세팅정보가 존재하겠죠?
		GameEngineDevice::RenderStart();

		// 매프레임마다 세팅해줘야 하는게 되죠?
		// 
		Pipe->Rendering();
		GameEngineDevice::RenderEnd();
	}
}


