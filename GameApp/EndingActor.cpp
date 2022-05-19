#include "PreCompile.h"
#include "EndingActor.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineLevel.h>

EndingActor::EndingActor()
	: renderer_(nullptr)
	, bgmPlayer_(nullptr)
{

}

EndingActor::~EndingActor()
{
	if (bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Stop();
	}
}

void EndingActor::Start()
{
	bgmPlayer_ = std::make_unique<GameEngineSoundPlayer>("MUS_GoodEnding.wav");
	bgmPlayer_->Play();

	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraComponent());
	renderer_->CreateAnimationFolder("TheEnd", 5.0f, false, false);
	renderer_->SetScale(1280.f, 720.f);
	renderer_->SetLocationZ(4.0f);
	renderer_->ChangeAnimation("TheEnd");

	{
		GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
		float4 color = float4::ONE;
		//float4 color = float4::ONE * -1.f;
		//color.r = 0.7f;
		//color.g = 0.7f;
		//color.b = 0.7f;
		color.a = 1.0f;
		r->SetColor(color);
		r->SetLocationZ(-10.0f);
		r->CreateAnimationFolder("OldFilmEffect", 0.034f, true, false);
		r->SetScale(1280.f, 720.f);
		r->ChangeAnimation("OldFilmEffect");
	}
}

void EndingActor::Update(float _deltaTime)
{
	if (renderer_->GetCurrentAnimation()->CurFrame_ >= 1)
	{
		renderer_->GetCurrentAnimation()->InterTime_ = 0.034f;
	}
	//GameEngineRandom random;
	//int soundNumber = random.RandomInt(1, 3);
	//std::string soundName = "cutscene_pageturn_0" + std::to_string(soundNumber) + ".wav";
	//GameEngineSoundManager::GetInstance().PlaySoundByName(soundName);
}
