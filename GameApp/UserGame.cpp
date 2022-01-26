#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>

#include <GameEngine/GameEngineManagerHelper.h>

#include "Macro.h"

UserGame::UserGame()
{

}

UserGame::~UserGame()
{

}

UserGame::UserGame(UserGame&& _other) noexcept
{

}

void UserGame::Initialize()
{
}


float4 pos;
float4 rot;

void UserGame::LoadResource()
{
	std::vector<float4> RectVertex;
	RectVertex.resize(4 * 6);
	{
		// ¾Õ¸é
		RectVertex[0] = float4({ -0.5f, 0.5f, 0.5f });
		RectVertex[1] = float4({ 0.5f, 0.5f, 0.5f });
		RectVertex[2] = float4({ 0.5f, -0.5f, 0.5f });
		RectVertex[3] = float4({ -0.5f, -0.5f, 0.5f });

		// µÞ¸é
		RectVertex[4] = float4::RotateXDegree(RectVertex[0], 180.0f);
		RectVertex[5] = float4::RotateXDegree(RectVertex[1], 180.0f);
		RectVertex[6] = float4::RotateXDegree(RectVertex[2], 180.0f);
		RectVertex[7] = float4::RotateXDegree(RectVertex[3], 180.0f);
	}

	{
		RectVertex[8] = float4::RotateYDegree(RectVertex[0], 90.0f);
		RectVertex[9] = float4::RotateYDegree(RectVertex[1], 90.0f);
		RectVertex[10] = float4::RotateYDegree(RectVertex[2], 90.0f);
		RectVertex[11] = float4::RotateYDegree(RectVertex[3], 90.0f);

		RectVertex[12] = float4::RotateYDegree(RectVertex[0], -90.0f);
		RectVertex[13] = float4::RotateYDegree(RectVertex[1], -90.0f);
		RectVertex[14] = float4::RotateYDegree(RectVertex[2], -90.0f);
		RectVertex[15] = float4::RotateYDegree(RectVertex[3], -90.0f);
	}

	{
		RectVertex[16] = float4::RotateXDegree(RectVertex[0], 90.0f);
		RectVertex[17] = float4::RotateXDegree(RectVertex[1], 90.0f);
		RectVertex[18] = float4::RotateXDegree(RectVertex[2], 90.0f);
		RectVertex[19] = float4::RotateXDegree(RectVertex[3], 90.0f);

		RectVertex[20] = float4::RotateXDegree(RectVertex[0], -90.0f);
		RectVertex[21] = float4::RotateXDegree(RectVertex[1], -90.0f);
		RectVertex[22] = float4::RotateXDegree(RectVertex[2], -90.0f);
		RectVertex[23] = float4::RotateXDegree(RectVertex[3], -90.0f);
	}

	VertexBufferManager.Create("Box", RectVertex);

	{
		std::vector<int> RectIndex;

		for (int i = 0; i < 6; i++)
		{
			RectIndex.push_back(i * 4 + 0);
			RectIndex.push_back(i * 4 + 1);
			RectIndex.push_back(i * 4 + 2);

			RectIndex.push_back(i * 4 + 0);
			RectIndex.push_back(i * 4 + 2);
			RectIndex.push_back(i * 4 + 3);
		}

		IndexBufferManager.Create("Box", RectIndex);
	}

	{
		VertexShaderManager.Create("BoxShader", 
			[](float4 _v)
			{
				float4x4 s;
				s.Scaling({ 20.f, 20.f, 20.f });

				float4x4 r;
				r.RotationDeg(rot * GameEngineMath::RadianToDegree);

				float4x4 t;
				t.Translation(pos);

				_v = _v * s * r * t;

				float4x4 view;
				view.ViewToLH({ 0, 0, -100 }, { 0, 0, 1 }, { 0, 1, 0 });

				float4x4 p;
				p.PerspectiveFovLH(90 * GameEngineMath::DegreeToRadian, 1270.f / 720.f, 0.001f, 1000.f);

				_v *= view * p;

				return _v;
			}
		);
	}

	GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("TestRasterizer");

	Ptr->SetViewPort(1280.0f, 720.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void UserGame::GameLoop()
{
	GameEngineRenderingPipeLine rp;


	rp.SetInputAssembler1("Box");
	rp.SetVertexShader("BoxShader");

	rp.SetInputAssembler2("Box");
	rp.SetRasterizer("TestRasterizer");

	float deltaTime = GameEngineTime::GetInst().GetDeltaTime();
	pos.x += deltaTime * 20;
	//rot.x += deltaTime * 3;
	//rot.y += deltaTime * 2;
	rot.z += deltaTime * 3;

	rp.Rendering();
}

void UserGame::Release()
{
}
