#include "PreCompile.h"
#include "TitleActor.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngineBase\GameEngineSoundManager.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineInput.h>

#include <GameEngine\GameEngineCore.h>
#include <GameApp\FadeOut.h>

TitleActor::TitleActor()
    : titleRenderer_(nullptr)
    , bgmPlayer_(nullptr)
{

}

TitleActor::~TitleActor()
{
    if (nullptr != bgmPlayer_)
    {
        bgmPlayer_->Stop();
        delete bgmPlayer_;
        bgmPlayer_ = nullptr;
    }
}

void TitleActor::Start()
{
    {
        GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>();
        float4 color;
        color.r = 0.3f;
        color.g = 0.3f;
        color.b = 0.3f;
        color.a = 0.3f;
        r->SetLocationZ(1.0f);
        r->SetColor(color);
        r->CreateAnimationFolder("OldFilmEffect", 0.034f, true, false);
        r->SetScale(1280.f, 720.f);
        r->ChangeAnimation("OldFilmEffect");
    }
    {
        GameEngineImageRenderer* r = CreateTransformComponent<GameEngineImageRenderer>(level_->GetMainCameraActor()->GetTransform());
        float4 color;
        color.r = 0.5f;
        color.g = 0.5f;
        color.b = 0.5f;
        color.a = 0.2f;
        r->SetColor(color);
        r->CreateAnimationFolder("OldFilmEffect", 0.034f, true, false);
        r->SetScale(1280.f, 720.f);
        r->SetLocationZ(1.0f);
        r->ChangeAnimation("OldFilmEffect");
    }

    GameEngineInput::GetInstance().CreateKey("P", 'P');
    GameEngineInput::GetInstance().CreateKey("Z", 'Z');

    bgmPlayer_ = new GameEngineSoundPlayer("MUS_Intro_DontDealWithDevil_Vocal.wav");
    state_.CreateState("Start", std::bind(&TitleActor::startStart, this, std::placeholders::_1), std::bind(&TitleActor::updateStart, this, std::placeholders::_1));
    state_.ChangeState("Start");

    titleRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
    titleRenderer_->SetTexture("title_screen_background.png", true);
    titleRenderer_->SetLocationZ(2.0f);

    GameEngineTransformComponent* cupheadTransform = CreateTransformComponent<GameEngineTransformComponent>(GetTransform());

    cupheadTransform->SetLocation(0.0f, -360.f, 0.0f);

    GameEngineImageRenderer* cupheadRenderer = CreateTransformComponent<GameEngineImageRenderer>(cupheadTransform);
    cupheadRenderer->CreateAnimationFolder("Idle", "Cuphead", 0.05f);
    cupheadRenderer->ChangeAnimation("Idle");
    cupheadRenderer->SetLocationZ(-1.0f);
}

void TitleActor::Update(float _deltaTime)
{
    state_.Update(_deltaTime);
}

void TitleActor::levelChangeEndEvent()
{
    bgmPlayer_->Stop();
}

void TitleActor::startStart(float _deltaTime)
{
    bgmPlayer_->Play();
    bgmPlayer_->SetVolume(0.5f);
}

void TitleActor::updateStart(float _deltaTime)
{
    //if (GameEngineInput::GetInstance().IsKeyDown("P"))
    //{
    //    bgmPlayer_->Stop();
    //    GameEngineCore::ChangeLevel("TutorialLevel");
    //}

    if (GameEngineInput::GetInstance().IsKeyDown("Z"))
    {
        state_ << "Press";
    }
}

void TitleActor::startPress(float _deltaTime)
{
    GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_WorldMap_LevelSelect_DiffucultySettings_Appear.wav");
    level_->CreateActor<FadeOut>();
}

void TitleActor::updatePress(float _deltaTime)
{
    if (state_.GetTime() > 0.55f)
    {
        bgmPlayer_->Stop();
        GameEngineCore::ChangeLevel("TutorialLevel");
    }
}
