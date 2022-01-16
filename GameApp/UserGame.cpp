#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine/GameEngineVertexShaderManager.h>
#include <GameEngine/GameEngineVertexBufferManager.h>
#include <GameEngine/GameEngineIndexBufferManager.h>

UserGame::UserGame() // default constructer 디폴트 생성자
{

}

UserGame::~UserGame() // default destructer 디폴트 소멸자
{

}

UserGame::UserGame(UserGame&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



void UserGame::Initialize() 
{
	GameEngineDebug::LeakCheckOn();
	GameEngineSound::GetInst().Initialize();
	return;
}

float RotAngle = 0.0f;
float4 BoxPos = { 0.0f, 0.0f, 0.0f };

void UserGame::ResourcesLoad() 
{
//	{
//		GameEngineDirectroy SoundDir;
//		SoundDir.MoveParent("Direct2D");
//		SoundDir.MoveChild("Resources");
//		SoundDir.MoveChild("Sound");
//
//		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("mp3");
//
//		for (size_t i = 0; i < AllFile.size(); i++)
//		{
//			GameEngineSound::GetInst().CreateSound(AllFile[i].GetFullPath(), GameEnginePath::GetFileName(AllFile[i].GetFullPath()));
//		}
//	}

	std::vector<float4> vertices;


	{
		// 각자 물체가 각자의 크기와 회전값을 가진 세상
		// 로컬스페이스

		// 로컬세상에 있는 물체를 우리가 원하는 대로 변형하고
		// 위치시키고 인식합니다.
		// 월드스페이스

		std::vector<float4> RectVertex = std::vector<float4>(4 * 6);

		{
			RectVertex[0] = float4({ -0.5f, 0.5f, 0.5f });
			RectVertex[1] = float4({ 0.5f, 0.5f, 0.5f });
			RectVertex[2] = float4({ 0.5f, -0.5f, 0.5f });
			RectVertex[3] = float4({ -0.5f, -0.5f, 0.5f });

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

		GameEngineVertexBufferManager::GetInst().Create("Rect", RectVertex);
	}

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



		GameEngineIndexBufferManager::GetInst().Create("Rect", RectIndex);
	}

	{

		GameEngineVertexShaderManager::GetInst().Create("TestShader", [](const float4& _Value)
			{

				float4 Pos = _Value;
				float4 WorldScale = { 100.0f, 100.0f, 100.0f };
				float4 WorldMove = { 100.0f, 0.0f };
				float4 WorldRot = { 0.0f, 0.0f, RotAngle };
				Pos *= WorldScale;
				Pos.RotateXDegree(WorldRot.x);
				Pos.RotateYDegree(WorldRot.y);
				Pos.RotateZDegree(WorldRot.z);
				Pos += BoxPos;

				// 거의 대부분을 버텍스 쉐이더에서 합니다.


				// 한번더 뭘로 변환시키고

				// 월드 세상에 위치시키기 위한 변형
				//float4 SpaceScale = { 1.0f, -1.0f, 1.0f };
				//float4 SpaceRot = { 0.0f, 0.0f, 0.0f };
				//float4 SpaceMove = { 1280.0f * 0.5f, 720*0.5f, 0.0f};

				//Pos *= SpaceScale;
				//Pos.RotateXDegree(SpaceRot.x);
				//Pos.RotateYDegree(SpaceRot.y);
				//Pos.RotateZDegree(SpaceRot.z);
				//Pos += SpaceMove;






				return Pos;
			}
		);
	}

}

void UserGame::Release() 
{
	GameEngineVertexBufferManager::Destroy();
	GameEngineIndexBufferManager::Destroy();
	GameEngineVertexShaderManager::Destroy();

	GameEngineSound::Destroy();
	GameEngineWindow::Destroy();
	GameEngineTime::Destroy();
}

void UserGame::GameLoop()
{
	GameEngineRenderingPipeLine Pipe;

	Pipe.SetInputAssembler1("Rect");
	Pipe.SetVertexShader("TestShader");
	Pipe.SetInputAssembler2("Rect");

	RotAngle += 360.0f * GameEngineTime::GetInst().GetDeltaTime();
	BoxPos.y += 10.0f * GameEngineTime::GetInst().GetDeltaTime();

	Pipe.Rendering();
}