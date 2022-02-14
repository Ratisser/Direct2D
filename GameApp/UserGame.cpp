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

float4 pos;
float4 rot;
TransformData TransData;

void UserGame::Initialize()
{
	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("BoxRendering");
	// cpu의 데이터와 상수버퍼를 연결한다.
	Pipe->ShaderHelper.SettingConstantBufferLink("TransformData", TransData);


	//Pipe->ShaderHelper.SettingConstantBufferLink("TransformData222", TransData);
	// cpu의 데이터와 상수버퍼를 한번 복사한다.
	// Pipe->ShaderHelper.SettingConstantBufferSet("TransformData", TransData);

	return;
}

void UserGame::Release()
{

}


//void UserGame::GameLoop()
//{
//	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("BoxRendering");
//	{
//		float deltaTime = GameEngineTime::GetInst().GetDeltaTime();
//		//pos.x += deltaTime * 20;
//		rot.x += deltaTime * 3;
//		rot.y += deltaTime * 2;
//		rot.z += deltaTime * 3;
//
//		float4x4 s;
//		s.Scaling({ 20.f, 20.f, 20.f });
//		float4x4 r;
//		r.RotationDeg(rot * GameEngineMath::RadianToDegree);
//		float4x4 t;
//		t.Translation(pos);
//
//		TransData.World = s * r * t;
//		TransData.View.ViewToLH({ 0.f,0.f,-100.f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
//		TransData.Proj.PerspectiveFovLH(GameEngineMath::DegreeToRadian * 90.f, 1280.f / 720.f, 0.001f, 1000.f);
//
//		// 랜더링 파이프라인이 하나가 돌고
//		// 이안에 세팅정보가 존재하겠죠?
//		GameEngineDevice::RenderStart();
//
//		// 매프레임마다 세팅해줘야 하는게 되죠?
//		// 
//		Pipe->Rendering();
//		GameEngineDevice::RenderEnd();
//	}
//}
//
//
