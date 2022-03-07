#include "PreCompile.h"
#include "GameEngineImageRenderer.h"
#include "GameEngineTextureManager.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineFolderTexture.h"

void GameEngineImageRenderer::Animation2D::CallStart() 
{
	for (auto& CallBack : StartCallBack_)
	{
		CallBack();
	}
}
void GameEngineImageRenderer::Animation2D::CallEnd() 
{
	for (auto& CallBack : EndCallBack_)
	{
		CallBack();
	}
}
void GameEngineImageRenderer::Animation2D::CallFrame() 
{
	for (auto& CallBack : FrameCallBack_)
	{
		if (CallBack.first != CurFrame_)
		{
			continue;
		}

		if (CallBack.second.size() == 0)
		{
			continue;
		}

		for (size_t i = 0; i < CallBack.second.size(); i++)
		{
			CallBack.second[i]();
		}
	}
}

void GameEngineImageRenderer::Animation2D::Reset() 
{
	IsEnd = false;
	CurTime_ = InterTime_;
	CurFrame_ = StartFrame_;
}

void GameEngineImageRenderer::Animation2D::Update(float _DeltaTime)
{
	CurTime_ -= _DeltaTime;

	if (CurTime_ <= 0.0f)
	{
		++CurFrame_;
		CurTime_ = InterTime_;
		if (true == Loop_
			&& CurFrame_ > EndFrame_)
		{
			CallEnd();
			CurFrame_ = StartFrame_;
		}
		else if (false == Loop_
			&& CurFrame_ > EndFrame_)
		{
			if (false == IsEnd)
			{
				CallEnd();
			}

			IsEnd = true;

			CurFrame_ = EndFrame_;
		}
	}

	CallFrame();
	if (nullptr == FolderTextures_)
	{
		Renderer->SetIndex(CurFrame_);
	} else 
	{
		Renderer->CutData_ = float4(0,0,1,1);
		GameEngineTexture* tex = Renderer->ShaderHelper_.SettingTexture("Tex", FolderTextures_->GetTextureIndex(CurFrame_));
		float4 size = tex->GetTextureSize();
		Renderer->SetScale(size);
		Renderer->SetLocation(0.0f, size.y / 2.f);
	}

}

/// ///////////////////////////////////////////////////////////////////

GameEngineImageRenderer::GameEngineImageRenderer()
	: CutData_(0, 0, 1, 1)
	, CurAnimation_(nullptr)
	, CurTexture_(nullptr)
{
}

GameEngineImageRenderer::~GameEngineImageRenderer() 
{
	for (auto& Animation : AllAnimations_)
	{
		if (nullptr == Animation.second)
		{
			continue;
		}

		delete Animation.second;
		Animation.second = nullptr;
	}
}

void GameEngineImageRenderer::Start()
{
	GameEngineRenderer::Start();

	SetRenderingPipeline("TextureBox");

	ShaderHelper_.SettingConstantBufferLink("TextureCutData", CutData_);
}

void GameEngineImageRenderer::SetIndex(const int Index) 
{
	if (nullptr == CurTexture_)
	{
		GameEngineDebug::MsgBoxError("텍스처가 존재하지 않는데 인덱스를 지정하려고 했습니다");
	}

	if (false == CurTexture_->IsCut())
	{
		GameEngineDebug::MsgBoxError("잘리지 않은 텍스처의 인덱스를 지정하려고 했습니다.");
	}

	CutData_ = CurTexture_->GetCutData(Index);

}

void GameEngineImageRenderer::SetImage(const std::string& _ImageName) 
{
	CurTexture_ = GameEngineTextureManager::GetInst().Find(_ImageName);

	if (nullptr == CurTexture_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처를 세팅하려고 했습니다");
		return;
	}

	ShaderHelper_.SettingTexture("Tex", _ImageName);
}

void GameEngineImageRenderer::CreateAnimation(const std::string& _Name, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop /*= true*/)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() != FindIter)
	{
		GameEngineDebug::MsgBoxError("이미 존재하는 애니메이션을 또 만들었습니다.");
	}

	Animation2D* NewAnimation = new Animation2D();

	NewAnimation->IsEnd = false;
	NewAnimation->Loop_ = _Loop;
	NewAnimation->InterTime_ = _InterTime;
	NewAnimation->CurTime_ = _InterTime;

	NewAnimation->FolderTextures_ = nullptr;
	NewAnimation->CurFrame_ = _StartFrame;
	NewAnimation->EndFrame_ = _EndFrame;
	NewAnimation->StartFrame_ = _StartFrame;
	NewAnimation->Renderer = this;

	AllAnimations_.insert(std::map<std::string, Animation2D*>::value_type(_Name, NewAnimation));
}

void GameEngineImageRenderer::CreateAnimationFolder(const std::string& _Name, const std::string& _FolderTexName, float _InterTime, bool _Loop /*= true*/)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() != FindIter)
	{
		GameEngineDebug::MsgBoxError("이미 존재하는 애니메이션을 또 만들었습니다.");
	}

	GameEngineFolderTexture* FolderTexture = GameEngineFolderTextureManager::GetInst().Find(_FolderTexName);
	
	if (nullptr == FolderTexture)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 폴더 텍스처를 세팅하려고 했습니다..");
	}


	Animation2D* NewAnimation = new Animation2D();

	NewAnimation->IsEnd = false;
	NewAnimation->Loop_ = _Loop;
	NewAnimation->InterTime_ = _InterTime;
	NewAnimation->CurTime_ = _InterTime;

	NewAnimation->FolderTextures_ = FolderTexture;
	NewAnimation->CurFrame_ = 0;
	NewAnimation->EndFrame_ = FolderTexture->GetTextureCount() - 1;
	NewAnimation->StartFrame_ = 0;
	NewAnimation->Renderer = this;

	AllAnimations_.insert(std::map<std::string, Animation2D*>::value_type(_Name, NewAnimation));
}

void GameEngineImageRenderer::SetChangeAnimation(const std::string& _Name, bool _IsForce /*= false*/)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션을 세팅하려고 했습니다");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("애니메이션의 애니메이션 nullptr 입니다");
	}

	if (false == _IsForce && CurAnimation_ == FindIter->second)
	{
		return;
	}
	
	CurAnimation_ = FindIter->second;

	CurAnimation_->Reset();
	CurAnimation_->CallStart();
}

void GameEngineImageRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation_)
	{
		return;
	}

	CurAnimation_->Update(_DeltaTime);
}

void GameEngineImageRenderer::SetStartCallBack(const std::string& _Name, std::function<void()> _CallBack)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션을 세팅하려고 했습니다");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("애니메이션의 애니메이션 nullptr 입니다");
	}

	FindIter->second->StartCallBack_.push_back(_CallBack);
}
void GameEngineImageRenderer::SetEndCallBack(const std::string& _Name, std::function<void()> _CallBack)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션을 세팅하려고 했습니다");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("애니메이션의 애니메이션 nullptr 입니다");
	}

	FindIter->second->EndCallBack_.push_back(_CallBack);
}
void GameEngineImageRenderer::SetFrameCallBack(const std::string& _Name, int _Index, std::function<void()> _CallBack)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션을 세팅하려고 했습니다");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("애니메이션의 애니메이션 nullptr 입니다");
	}

	FindIter->second->FrameCallBack_[_Index].push_back(_CallBack);
}