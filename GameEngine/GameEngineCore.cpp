#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineManagerHelper.h"
#include "GameEngineDevice.h"
#include "GameEngineInput.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngine/GameEngineCollision.h>

GameEngineCore* GameEngineCore::MainCore_ = nullptr;
std::map<std::string, GameEngineLevel*> GameEngineCore::allLevels_ = std::map<std::string, GameEngineLevel*>();
GameEngineLevel* GameEngineCore::nextLevel_ = nullptr;
GameEngineLevel* GameEngineCore::currentLevel_ = nullptr;

GameEngineCore::GameEngineCore() // default constructer ����Ʈ ������
{

}

GameEngineCore::~GameEngineCore() // default destructer ����Ʈ �Ҹ���
{

}

GameEngineCore::GameEngineCore(GameEngineCore&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{


}

void GameEngineCore::EngineInitialize()
{
	loadEngineResource();
	GameEngineCollision::init();

	GameEngineSoundManager::GetInstance().Initialize();
	GameEngineInput::GetInstance();
}


void GameEngineCore::EngineDestroy()
{
	for (std::pair<const std::string, GameEngineLevel*>& p : allLevels_)
	{
		if (nullptr != p.second)
		{
			delete p.second;
			p.second = nullptr;
		}
	}

	GameEngineManagerHelper::ManagerRelease();
	GameEngineTime::Destroy();
	GameEngineDevice::Destroy();
	GameEngineInput::Destroy();
	GameEngineWindow::Destroy();
}

void GameEngineCore::loadEngineTexture()
{
	GameEngineDirectory EngineTextureDir;
	EngineTextureDir.MoveParent("Direct2D");
	EngineTextureDir.MoveChild("EngineResources");
	EngineTextureDir.MoveChild("Texture");

	std::vector<GameEngineFile> AllFile = EngineTextureDir.GetAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	}
}

void GameEngineCore::loadEngineShader()
{
	GameEngineDirectory Dir;
	Dir.MoveParent("Direct2D");
	Dir.MoveChild("EngineResources");
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

void GameEngineCore::loadEngineResource()
{
	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };

		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		// �����Ǳ׷���
		// Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		// �ð�ݴ�������� �׷����͵��� �׷���
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;

		//// ȭ�� �ٱ��� ���� ����� �߶󳽴�.
		// Info.FrontCounterClockwise = true;
		//Info.ScissorEnable = true;
		//Info.SlopeScaledDepthBias = 0;

		//// ���̰����� ���� �����Ұ̴ϴ�.
		//// ���̹��۸� �����ϰ� ���� �մϴ�.
		//Info.DepthBias = 0;
		//Info.DepthBiasClamp = 0;
		//Info.DepthClipEnable = FALSE;
		//Info.MultisampleEnable = TRUE;

		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizer", Info);
		Ptr->SetViewPort(1280.0f, 720.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Ptr = GameEngineRasterizerManager::GetInst().Create("NoCullRasterizer", Info);
		Ptr->SetViewPort(1280.0f, 720.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	{
		D3D11_BLEND_DESC bd;

		bd.AlphaToCoverageEnable = false;
		bd.IndependentBlendEnable = false;

		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		bd.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GameEngineBlenderManager::GetInst().Create("AlphaBlend", bd);
	}



	{
		D3D11_BLEND_DESC bd;

		bd.AlphaToCoverageEnable = true;
		bd.IndependentBlendEnable = false;

		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		bd.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GameEngineBlenderManager::GetInst().Create("DefaultBlend", bd);
	}

}

void GameEngineCore::MainLoop()
{
	GameEngineTime::GetInst().TimeCheck();
	GameEngineSoundManager::GetInstance().Update();
	GameEngineInput::GetInstance().update();

	if (nullptr != nextLevel_)
	{
		if (nullptr != currentLevel_)
		{
			currentLevel_->LevelChangeEndEvent();
			currentLevel_->levelChangeEndActorEvent();
		}

		nextLevel_->LevelChangeStartEvent();
		nextLevel_->levelChangeStartActorEvent();
		currentLevel_ = nextLevel_;
		nextLevel_ = nullptr;
		GameEngineTime::GetInst().TimeCheck();
	}

	if (nullptr == currentLevel_)
	{
		GameEngineDebug::MsgBoxError("���� ������ �������� �ʽ��ϴ�.");
	}


	float deltaTime = GameEngineTime::GetInst().GetDeltaTime();

	currentLevel_->LevelUpdate(deltaTime);
	currentLevel_->ActorUpdate(deltaTime);
	currentLevel_->LevelUpdateAfter(deltaTime);
	currentLevel_->Render();
	currentLevel_->Release(deltaTime);
}

void GameEngineCore::WindowCreate(GameEngineCore& _RuntimeCore)
{
	GameEngineWindow::GetInst().CreateMainWindow("MainWindow", _RuntimeCore.StartWindowSize(), _RuntimeCore.StartWindowPos());

	// ����̽��� ��������� �մϴ�.
	// HWND �����쿡�� �����ϴ� 3D ���̺귯���ϱ� WINDOW API�� ������� ó���Ǿ� �ֽ��ϴ�.
	GameEngineDevice::GetInst().Initialize();
}

void GameEngineCore::Loop()
{
	GameEngineWindow::GetInst().Loop(&GameEngineCore::MainLoop);
}


void GameEngineCore::ChangeLevel(const std::string& _levelName)
{
	std::map<std::string, GameEngineLevel*>::iterator findIter = allLevels_.find(_levelName);

	if (findIter == allLevels_.end())
	{
		GameEngineDebug::MsgBoxError("There is no level named \"" + _levelName + "\"");
	}

	nextLevel_ = findIter->second;
}