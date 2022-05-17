#include "PreCompile.h"
#include "FadeOut.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

FadeOut::FadeOut()
	: renderer_(nullptr)
{

}

FadeOut::~FadeOut()
{

}

void FadeOut::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
	renderer_->CreateAnimationFolderReverse("Iris", 0.034f, false, false);
	renderer_->SetScale(1280.f, 720.f);
	renderer_->SetLocationZ(-4.0f);

	renderer_->ChangeAnimation("Iris");
	
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_WorldMap_LevelSelect_DiffucultySettings_Appear.wav");

	SetBulletTimeEffect(false);
}

void FadeOut::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		Release();
	}
}
