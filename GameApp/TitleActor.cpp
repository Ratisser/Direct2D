#include "PreCompile.h"
#include "TitleActor.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngineBase\GameEngineSoundManager.h>
#include <GameEngineBase\GameEngineSoundPlayer.h>

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineInput.h>

#include <GameEngine\GameEngineCore.h>

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
    GameEngineInput::GetInstance().CreateKey("P", 'P');

    bgmPlayer_ = new GameEngineSoundPlayer("MUS_Intro_DontDealWithDevil_Vocal.wav");
    state_.CreateState("Start", std::bind(&TitleActor::startStart, this, std::placeholders::_1), std::bind(&TitleActor::updateStart, this, std::placeholders::_1));
    state_.ChangeState("Start");

    titleRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
    titleRenderer_->SetTexture("title_screen_background.png", true);

    GameEngineTransformComponent* cupheadTransform = CreateTransformComponent<GameEngineTransformComponent>(GetTransform());

    cupheadTransform->SetLocation(0.0f, -360.f, -1.0f);

    GameEngineImageRenderer* cupheadRenderer = CreateTransformComponent<GameEngineImageRenderer>(cupheadTransform);
    cupheadRenderer->CreateAnimationFolder("Idle", "Cuphead", 0.05f);
    cupheadRenderer->ChangeAnimation("Idle");
}

void TitleActor::Update(float _deltaTime)
{
    state_.Update(_deltaTime);
}

void TitleActor::startStart(float _deltaTime)
{
    bgmPlayer_->Play();
    bgmPlayer_->SetVolume(0.5f);
}

void TitleActor::updateStart(float _deltaTime)
{
    if (GameEngineInput::GetInstance().IsKeyDown("P"))
    {
        bgmPlayer_->Stop();
        GameEngineCore::ChangeLevel("PlayLevel");
    }
}
