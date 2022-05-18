#include "PreCompile.h"
#include "LoadingLevel.h"

#include <GameEngineBase\GameEngineSoundManager.h>
#include <GameEngine\GameEngineCore.h>
#include <GameEngine\GameEngineWindow.h>
#include <GameEngine\GameEngineRenderingPipeline.h>

#include <GameApp\TutorialLevel.h>
#include <GameApp\TitleLevel.h>
#include <GameApp\PlayLevel.h>
#include <GameApp\DevilLevel.h>
#include <GameApp\WorldLevel.h>
#include <GameApp\CustomVertex.h>

#include "Hourglass.h"
#include <GameApp\EndingLevel.h>
#include <GameApp\FlowerLevel.h>

LoadingLevel::LoadingLevel()
	: bLoading_(false)
	, bFinish_(false)
	, bLevelLoadFinish_(false)
	, timeCounter_(0.0f)
{

}

LoadingLevel::~LoadingLevel()
{

}

void LoadingLevel::LevelChangeEndEvent()
{
}

void LoadingLevel::LevelChangeStartEvent()
{
}

void LoadingLevel::LevelStart()
{
	mainCamera_->GetCameraComponent()->SetLocationZ(-420.f);
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
	CreateActor<Hourglass>();
}

void LoadingLevel::LevelUpdate(float _deltaTime)
{
	timeCounter_ += _deltaTime;


	if (bFinish_)
	{
		GameEngineCore::ChangeLevel("TitleLevel");
	}

	if (bLoading_ == false)
	{
		loadSound();
		loadShader();
		loadTexture();
		loadMesh();

		loadPipeLine();

		Sleep(50);
		bLoading_ = true;
	}

	if (GameEngineCore::ThreadQueue_.GetWorkingCount() > 0)
	{
		Sleep(1);
	}
	else if (bLevelLoadFinish_ == false)
	{
		loadLevel();

		GameEngineDebug::OutPutDebugString("Load finish\nElapsedTime : " + std::to_string(timeCounter_) + "\n");

		bLevelLoadFinish_ = true;
		bFinish_ = true;
	}

}

void LoadingLevel::loadLevel()
{
	GameEngineCore::CreateLevel<TutorialLevel>("TutorialLevel");
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<PlayLevel>("PlayLevel");
	GameEngineCore::CreateLevel<WorldLevel>("WorldLevel");
	GameEngineCore::CreateLevel<DevilLevel>("DevilLevel");
	GameEngineCore::CreateLevel<EndingLevel>("EndingLevel");
	GameEngineCore::CreateLevel<FlowerLevel>("FlowerLevel");
}

void LoadingLevel::loadSound()
{
	GameEngineCore::ThreadQueue_.JobPost(
		[]()
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
		});

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
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
		});
}

void LoadingLevel::loadShader()
{
}

void LoadingLevel::loadTexture()
{
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

	GameEngineCore::ThreadQueue_.JobPost(
		[]()
		{
			GameEngineDirectory Dir;
			Dir.MoveParent("Direct2D");
			Dir / "Resources" / "Image" / "Devil" / "Devil" / "Phase2";

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
			Dir / "Resources" / "Image" / "Devil" / "Devil" / "Phase3";

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
			Dir / "Resources" / "Image" / "Devil" / "Devil" / "Demon";

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
			Dir / "Resources" / "Image" / "Effect";

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
			Dir / "Resources" / "Image" / "Flower" / "Boss_flower";

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
			Dir / "Resources" / "Image" / "Flower" / "Background";

			std::vector<GameEngineFile> AllFile = Dir.GetAllFileWithoutDirectory();

			for (size_t i = 0; i < AllFile.size(); i++)
			{
				GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
			}
		}
	);

	Sleep(50);
}

void LoadingLevel::loadMesh()
{
}

void LoadingLevel::loadPipeLine()
{
}
