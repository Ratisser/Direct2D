#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>

UserGame::UserGame() // default constructer ����Ʈ ������
{

}

UserGame::~UserGame() // default destructer ����Ʈ �Ҹ���
{

}

UserGame::UserGame(UserGame&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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
	// cpu�� �����Ϳ� ������۸� �����Ѵ�.
	Pipe->ShaderHelper.SettingConstantBufferLink("TransformData", TransData);


	//Pipe->ShaderHelper.SettingConstantBufferLink("TransformData222", TransData);
	// cpu�� �����Ϳ� ������۸� �ѹ� �����Ѵ�.
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
//		// ������ ������������ �ϳ��� ����
//		// �̾ȿ� ���������� �����ϰ���?
//		GameEngineDevice::RenderStart();
//
//		// �������Ӹ��� ��������� �ϴ°� ����?
//		// 
//		Pipe->Rendering();
//		GameEngineDevice::RenderEnd();
//	}
//}
//
//
