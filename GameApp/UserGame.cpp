#include "PreCompile.h"
#include "UserGame.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEngineSoundManager.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeline.h>

#include <GameApp/TitleLevel.h>
#include <GameApp/TitleLevel.h>
#include <GameApp/PlayLevel.h>
#include <GameApp/TutorialLevel.h>
#include <GameApp\WorldLevel.h>
#include <GameApp\CustomVertex.h>

#include "DevilLevel.h"

UserGame::UserGame() // default constructer 디폴트 생성자
{

}

UserGame::~UserGame() // default destructer 디폴트 소멸자
{

}

void UserGame::Initialize()
{
}

void UserGame::Release()
{

}

void UserGame::loadLevel()
{
	CreateLevel<TutorialLevel>("TutorialLevel");
	CreateLevel<TitleLevel>("TitleLevel");
	CreateLevel<PlayLevel>("PlayLevel");
	CreateLevel<WorldLevel>("WorldLevel");
	CreateLevel<DevilLevel>("DevilLevel");
	//ChangeLevel("TitleLevel");
	//ChangeLevel("TutorialLevel");
	//ChangeLevel("PlayLevel");
	//ChangeLevel("WorldLevel");
	ChangeLevel("DevilLevel");
}

void UserGame::loadSound()
{
	GameEngineSoundManager::GetInstance().SetGlobalVolume(0.5f);

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("Direct2D");
		SoundDir.MoveChild("Resources");
		SoundDir.MoveChild("Sound");
		SoundDir.MoveChild("Music");

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance().CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("Direct2D");
		SoundDir.MoveChild("Resources");
		SoundDir.MoveChild("Sound");
		SoundDir.MoveChild("Sfx");

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance().CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}
}

void UserGame::loadShader()
{
	GameEngineDirectory Dir;
	Dir.MoveParent("Direct2D");
	Dir.MoveChild("Resources");
	Dir.MoveChild("Shader");

	std::vector<GameEngineFile> AllShader = Dir.GetAllFile("fx");

	for (auto& ShaderFile : AllShader)
	{
		ShaderFile.Open("rt");

		std::string FileName = ShaderFile.GetFileNameWithOutExtension();
		std::string AllCode = ShaderFile.GetString();

		if (std::string::npos != AllCode.find(FileName + "_VS"))
		{
			GameEngineVertexShader* Ptr = GameEngineVertexShaderManager::GetInst().Load(FileName + "_VS", ShaderFile.GetFullPath(), FileName + "_VS");
		}

		if (std::string::npos != AllCode.find(FileName + "_PS"))
		{
			GameEnginePixelShader* Ptr = GameEnginePixelShaderManager::GetInst().Load(FileName + "_PS", ShaderFile.GetFullPath(), FileName + "_PS");
		}

	}
}

void UserGame::loadTexture()
{
	GameEngineTime::GetInst().TimeCheckReset();
	GameEngineTime::GetInst().TimeCheck();
	GameEngineDebug::OutPutDebugString("\n" + std::to_string(GameEngineTime::GetInst().GetDeltaTime()) + "\n");

	//{
	//	GameEngineDirectory TextureDir;
	//	TextureDir.MoveParent("Direct2D");
	//	TextureDir.MoveChild("Resources");
	//	TextureDir.MoveChild("Image");

	//	std::vector<GameEngineFile> AllFile = TextureDir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory TextureDir;
	//	TextureDir.MoveParent("Direct2D");
	//	TextureDir.MoveChild("Resources");
	//	TextureDir.MoveChild("Image");
	//	TextureDir.MoveChild("CupHead");

	//	std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();
	//	for (GameEngineFile& file : AllFile)
	//	{
	//		GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
	//	}

	//	TextureDir.MoveParent("Image");
	//	TextureDir / "Bullet";
	//	AllFile = TextureDir.GetAllFile();
	//	for (GameEngineFile& file : AllFile)
	//	{
	//		GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
	//	}

	//}



	//{
	//	GameEngineDirectory TextureDir;
	//	TextureDir.MoveParent("Direct2D");
	//	TextureDir.MoveChild("Resources");
	//	TextureDir.MoveChild("Image");
	//	TextureDir / "Title";
	//	std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();
	//	for (GameEngineFile& file : AllFile)
	//	{
	//		GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory TextureDir;
	//	TextureDir.MoveParent("Direct2D");
	//	TextureDir / "Resources" / "Image" / "Screen";
	//	std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();
	//	for (GameEngineFile& file : AllFile)
	//	{
	//		GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir.MoveChild("Resources");
	//	Dir.MoveChild("Image");
	//	Dir.MoveChild("Title");

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}


	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "Title" / "Cuphead";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "Tutorial" / "Background";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "Tutorial" / "Guides";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "World" / "Background";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "World" / "Cuphead";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "Devil" / "Background";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}



	//{
	//	GameEngineDirectory Dir;
	//	Dir.MoveParent("Direct2D");
	//	Dir / "Resources" / "Image" / "Devil" / "Devil" / "Phase1";

	//	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	//	}
	//}

	

	// 이 밑으로 스레드
	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("Direct2D");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");

			std::vector<GameEngineFile> AllFile = TextureDir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("Direct2D");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir.MoveChild("CupHead");

			std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();
			for (GameEngineFile& file : AllFile)
			{
				GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
			}

			TextureDir.MoveParent("Image");
			TextureDir / "Bullet";
			AllFile = TextureDir.GetAllFile();
			for (GameEngineFile& file : AllFile)
			{
				GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
			}

		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("Direct2D");
			TextureDir.MoveChild("Resources");
			TextureDir.MoveChild("Image");
			TextureDir / "Title";
			std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();
			for (GameEngineFile& file : AllFile)
			{
				GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory TextureDir;
			TextureDir.MoveParent("Direct2D");
			TextureDir / "Resources" / "Image" / "Screen";
			std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();
			for (GameEngineFile& file : AllFile)
			{
				GameEngineFolderTextureManager::GetInst().Load(file.GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir.MoveChild("Resources");
			Dir.MoveChild("Image");
			Dir.MoveChild("Title");

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);
	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "Title" / "Cuphead";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "Tutorial" / "Background";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "Tutorial" / "Guides";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "World" / "Background";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "World" / "Cuphead";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "Devil" / "Background";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "Devil" / "Devil" / "Phase1";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineFolderTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	Sleep(100);

	while (GameEngineCore::ThreadQueue_.GetWorkingCount() > 0)
	{
		Sleep(1);
	}

	GameEngineTime::GetInst().TimeCheck();
	GameEngineDebug::OutPutDebugString("\n" + std::to_string(GameEngineTime::GetInst().GetDeltaTime()) + "\n");
}

void UserGame::loadMesh()
{
	// Box
	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4 * 6);

		{
			// 앞면
			RectVertex[0] = { float4({ -0.5f, 0.5f, -0.5f }) };
			RectVertex[1] = { float4({ 0.5f, 0.5f, -0.5f }) };
			RectVertex[2] = { float4({ 0.5f, -0.5f, -0.5f }) };
			RectVertex[3] = { float4({ -0.5f, -0.5f, -0.5f }) };

			RectVertex[4] = { float4::RotateXDegree(RectVertex[0].Position, 180.0f) };
			RectVertex[5] = { float4::RotateXDegree(RectVertex[1].Position, 180.0f) };
			RectVertex[6] = { float4::RotateXDegree(RectVertex[2].Position, 180.0f) };
			RectVertex[7] = { float4::RotateXDegree(RectVertex[3].Position, 180.0f) };
			//RectVertex[4] = { float4({ 0.5f, 0.5f, 0.5f }) };
			//RectVertex[5] = { float4({ -0.5f, 0.5f, 0.5f }) };
			//RectVertex[6] = { float4({ -0.5f, -0.5f, 0.5f }) };
			//RectVertex[7] = { float4({ 0.5f, -0.5f, 0.5f }) };
		}

		{
			RectVertex[8] = { float4::RotateYDegree(RectVertex[0].Position, 90.0f) };
			RectVertex[9] = { float4::RotateYDegree(RectVertex[1].Position, 90.0f) };
			RectVertex[10] = { float4::RotateYDegree(RectVertex[2].Position, 90.0f) };
			RectVertex[11] = { float4::RotateYDegree(RectVertex[3].Position, 90.0f) };

			RectVertex[12] = { float4::RotateYDegree(RectVertex[0].Position, -90.0f) };
			RectVertex[13] = { float4::RotateYDegree(RectVertex[1].Position, -90.0f) };
			RectVertex[14] = { float4::RotateYDegree(RectVertex[2].Position, -90.0f) };
			RectVertex[15] = { float4::RotateYDegree(RectVertex[3].Position, -90.0f) };
		}

		{
			RectVertex[16] = { float4::RotateXDegree(RectVertex[0].Position, 90.0f) };
			RectVertex[17] = { float4::RotateXDegree(RectVertex[1].Position, 90.0f) };
			RectVertex[18] = { float4::RotateXDegree(RectVertex[2].Position, 90.0f) };
			RectVertex[19] = { float4::RotateXDegree(RectVertex[3].Position, 90.0f) };

			RectVertex[20] = { float4::RotateXDegree(RectVertex[0].Position, -90.0f) };
			RectVertex[21] = { float4::RotateXDegree(RectVertex[1].Position, -90.0f) };
			RectVertex[22] = { float4::RotateXDegree(RectVertex[2].Position, -90.0f) };
			RectVertex[23] = { float4::RotateXDegree(RectVertex[3].Position, -90.0f) };
		}

		for (int i = 0; i < 6; i++)
		{
			if (i == 1)
			{
				RectVertex[i * 4 + 0].TexCoord = float4(0.0f, 1.0f);
				RectVertex[i * 4 + 1].TexCoord = float4(1.0f, 1.0f);
				RectVertex[i * 4 + 2].TexCoord = float4(1.0f, 0.0f);
				RectVertex[i * 4 + 3].TexCoord = float4(0.0f, 0.0f);
				continue;
			}
			RectVertex[i * 4 + 0].TexCoord = float4(0.0f, 0.0f);
			RectVertex[i * 4 + 1].TexCoord = float4(1.0f, 0.0f);
			RectVertex[i * 4 + 2].TexCoord = float4(1.0f, 1.0f);
			RectVertex[i * 4 + 3].TexCoord = float4(0.0f, 1.0f);
		}

		GameEngineVertexBufferManager::GetInst().Create("Box", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);

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
	}

	// Rect
	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// 앞면
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }) };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }) };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }) };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }) };

			RectVertex[0].TexCoord = float4(0.0f, 0.0f);
			RectVertex[1].TexCoord = float4(1.0f, 0.0f);
			RectVertex[2].TexCoord = float4(1.0f, 1.0f);
			RectVertex[3].TexCoord = float4(0.0f, 1.0f);
		}

		GameEngineVertexBufferManager::GetInst().Create("Rect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);

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
	}

	// DebugRect
	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(5);

		{
			// 앞면
			RectVertex[0] = { float4(-0.5f, 0.5f, 0.0f) };
			RectVertex[1] = { float4(0.5f, 0.5f, 0.0f) };
			RectVertex[2] = { float4(0.5f, -0.5f, 0.0f) };
			RectVertex[3] = { float4(-0.5f, -0.5f, 0.0f) };
			RectVertex[4] = { float4(-0.5f, 0.5f, 0.0f) };

			RectVertex[0].TexCoord = float4(0.0f, 0.0f);
			RectVertex[1].TexCoord = float4(1.0f, 0.0f);
			RectVertex[2].TexCoord = float4(1.0f, 1.0f);
			RectVertex[3].TexCoord = float4(0.0f, 1.0f);
			RectVertex[4].TexCoord = float4(0.0f, 0.0f);
		}

		GameEngineVertexBufferManager::GetInst().Create("DebugRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		{
			std::vector<UINT> RectIndex;

			RectIndex.push_back(0);
			RectIndex.push_back(1);
			RectIndex.push_back(2);
			RectIndex.push_back(3);
			RectIndex.push_back(4);

			GameEngineIndexBufferManager::GetInst().Create("DebugRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		}
	}

	// FullRect
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

			RectVertex[0].TexCoord = float4(0.0f, 0.0f);
			RectVertex[1].TexCoord = float4(1.0f, 0.0f);
			RectVertex[2].TexCoord = float4(1.0f, 1.0f);
			RectVertex[3].TexCoord = float4(0.0f, 1.0f);


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
}

void UserGame::loadPipeLine()
{
	// DebugRect
	{
		GameEngineRenderingPipeline* Pipe = GameEngineRenderingPipelineManager::GetInst().Create("DebugRect");

		// 이런 기본적인 vertex들이 있다.
		Pipe->SetInputAssembler1VertexBufferSetting("DebugRect");
		Pipe->SetInputAssembler1InputLayOutSetting("Color_VS");

		Pipe->SetInputAssembler2IndexBufferSetting("DebugRect");
		Pipe->SetVertexShader("Color_VS");

		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

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
		Pipe->SetRasterizer("NoCullRasterizer");

		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	// BoxRendering
	{
		GameEngineRenderingPipeline* Pipe = GameEngineRenderingPipelineManager::GetInst().Create("BoxRendering");

		Pipe->SetInputAssembler1VertexBufferSetting("Box");
		Pipe->SetInputAssembler1InputLayOutSetting("Color_VS");

		Pipe->SetVertexShader("Color_VS");

		Pipe->SetInputAssembler2IndexBufferSetting("Box");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pipe->SetRasterizer("EngineBaseRasterizer");

		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
		//Pipe->SetOutputMergerBlend("DefaultBlend");
	}

	// FullRect
	{
		GameEngineRenderingPipeline* Pipe = GameEngineRenderingPipelineManager::GetInst().Create("FullRect");

		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler1InputLayOutSetting("TextureNoCut_VS");
		Pipe->SetVertexShader("TextureNoCut_VS");
		Pipe->SetPixelShader("TextureNoCut_PS");

		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pipe->SetRasterizer("EngineBaseRasterizer");

		Pipe->SetOutputMergerBlend("AlphaBlend");
		//Pipe->SetOutputMergerBlend("DefaultBlend");
	}

	// TextureBox
	{
		GameEngineRenderingPipeline* Pipe = GameEngineRenderingPipelineManager::GetInst().Create("TextureBox");

		Pipe->SetInputAssembler1VertexBufferSetting("Box");
		Pipe->SetInputAssembler1InputLayOutSetting("Texture_VS");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetPixelShader("Texture_PS");

		Pipe->SetInputAssembler2IndexBufferSetting("Box");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pipe->SetRasterizer("EngineBaseRasterizer");

		Pipe->SetOutputMergerBlend("AlphaBlend");
		//Pipe->SetOutputMergerBlend("DefaultBlend");

	}
}

void UserGame::ResourcesLoad()
{
	loadSound();

	loadTexture();
	loadShader();

	loadMesh();
	loadPipeLine();

	loadLevel();
}
