#include "PreCompile.h"
#include "UserGame.h"
#include "CustomVertex.h"
#include "UserGame_Resources_Shader.h"


void UserGame::ResourcesLoad()
{
	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("AR38");
		SoundDir.MoveChild("Resources");
		SoundDir.MoveChild("Sound");

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("mp3");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInst().LoadSound(AllFile[i].GetFullPath());
		}
	}

	AppShaderLoad();

	{
		// 각자 물체가 각자의 크기와 회전값을 가진 세상
		// 로컬스페이스

		// 로컬세상에 있는 물체를 우리가 원하는 대로 변형하고
		// 위치시키고 인식합니다.
		// 월드스페이스

		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4 * 6);

		{
			// 앞면
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.5f }) };
			RectVertex[1] = {float4({ 0.5f, 0.5f, 0.5f }) };
			RectVertex[2] = {float4({ 0.5f, -0.5f, 0.5f }) };
			RectVertex[3] = {float4({ -0.5f, -0.5f, 0.5f }) };

			RectVertex[4] = { float4::RotateXDegree(RectVertex[0].Postion, 180.0f) };
			RectVertex[5] = { float4::RotateXDegree(RectVertex[1].Postion, 180.0f) };
			RectVertex[6] = { float4::RotateXDegree(RectVertex[2].Postion, 180.0f) };
			RectVertex[7] = { float4::RotateXDegree(RectVertex[3].Postion, 180.0f) };
		}

		{
			RectVertex[8] =  {float4::RotateYDegree(RectVertex[0].Postion, 90.0f)};
			RectVertex[9] =  {float4::RotateYDegree(RectVertex[1].Postion, 90.0f)};
			RectVertex[10] = { float4::RotateYDegree(RectVertex[2].Postion, 90.0f)};
			RectVertex[11] = { float4::RotateYDegree(RectVertex[3].Postion, 90.0f) };

			RectVertex[12] = {float4::RotateYDegree(RectVertex[0].Postion, -90.0f)};
			RectVertex[13] = {float4::RotateYDegree(RectVertex[1].Postion, -90.0f)};
			RectVertex[14] = {float4::RotateYDegree(RectVertex[2].Postion, -90.0f)};
			RectVertex[15] = {float4::RotateYDegree(RectVertex[3].Postion, -90.0f)};
		}

		{
			RectVertex[16] = {float4::RotateXDegree(RectVertex[0].Postion, 90.0f)};
			RectVertex[17] = {float4::RotateXDegree(RectVertex[1].Postion, 90.0f)};
			RectVertex[18] = {float4::RotateXDegree(RectVertex[2].Postion, 90.0f)};
			RectVertex[19] = {float4::RotateXDegree(RectVertex[3].Postion, 90.0f)};

			RectVertex[20] = {float4::RotateXDegree(RectVertex[0].Postion, -90.0f)};
			RectVertex[21] = {float4::RotateXDegree(RectVertex[1].Postion, -90.0f)};
			RectVertex[22] = {float4::RotateXDegree(RectVertex[2].Postion, -90.0f)};
			RectVertex[23] = { float4::RotateXDegree(RectVertex[3].Postion, -90.0f) };
		}

		GameEngineVertexBufferManager::GetInst().Create("Box", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		for (int i = 0; i < 6; i++)
		{
			RectIndex.push_back(i * 4 + 0);
			RectIndex.push_back(i * 4 + 1);
			RectIndex.push_back(i * 4 + 2);

			RectIndex.push_back(i * 4 + 0);
			RectIndex.push_back(i * 4 + 2);
			RectIndex.push_back(i * 4 + 3);
		}

		GameEngineIndexBufferManager::GetInst().Create("Box", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}



	{
		// 각자 물체가 각자의 크기와 회전값을 가진 세상
		// 로컬스페이스

		// 로컬세상에 있는 물체를 우리가 원하는 대로 변형하고
		// 위치시키고 인식합니다.
		// 월드스페이스

		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// 앞면
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }) };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }) };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }) };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }) };
		}

		GameEngineVertexBufferManager::GetInst().Create("Rect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("Rect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		// 각자 물체가 각자의 크기와 회전값을 가진 세상
		// 로컬스페이스

		// 로컬세상에 있는 물체를 우리가 원하는 대로 변형하고
		// 위치시키고 인식합니다.
		// 월드스페이스

		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// 앞면
			RectVertex[0] = { float4({ -1.0f, 1.0f, 0.0f }) };
			RectVertex[1] = { float4({ 1.0f, 1.0f, 0.0f }) };
			RectVertex[2] = { float4({ 1.0f, -1.0f, 0.0f }) };
			RectVertex[3] = { float4({ -1.0f, -1.0f, 0.0f }) };
		}

		GameEngineVertexBufferManager::GetInst().Create("FullRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("FullRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}



	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };

		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		// 무조건그려라
		// Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		// 시계반대방향으로 그려진것들을 그려라
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;

		//// 화면 바깥에 나간 면들을 잘라낸다.
		// Info.FrontCounterClockwise = true;
		//Info.ScissorEnable = true;
		//Info.SlopeScaledDepthBias = 0;

		//// 깊이관련은 추후 설명할겁니다.
		//// 깊이버퍼를 설명하고 들어야 합니다.
		//Info.DepthBias = 0;
		//Info.DepthBiasClamp = 0;
		//Info.DepthClipEnable = FALSE;
		//Info.MultisampleEnable = TRUE;

		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizer", Info);
		Ptr->SetViewPort(1280.0f, 720.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorRendering");

		// 이런 기본적인 vertex들이 있다.
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetInputAssembler1InputLayOutSetting("Color_VS");

		// 그 vertex을 이렇게 위치시키겠다.
		Pipe->SetVertexShader("Color_VS");

		// 그 vertex을 3개 묶어서 면으로 그리겠다. 순서는 인덱스 버퍼의 순서대로
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 헐 테셀레이션 도메인 지오메트리는 있으면 적용되고 없어도 필수는 아니다. 
		// vertex을 더 쪼갤건데 준비를 하겠다. 
		// 헐
		// 
		// 헐에서 정한대로 vertex를 더 쪼갠다.
		// 테셀레이션 
		// 
		// 그 더 쪼갠 vertex들을 수정하겠다.
		// 도메인 
		// 
		// 지오메트리 완전히 새로운 vertex들을 또 만들겠다.
		// 애는 게임에서 좀 많이 쓸모있음.

		// 그리리기한 면혹은 선 등등에 겹치는 모니터의 픽셀들을 추출하겠다. 
		// 레스터라이터라이저
		Pipe->SetRasterizer("EngineBaseRasterizer");

		Pipe->SetPixelShader("Color_PS");
	}



}
