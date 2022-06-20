#include "PreCompile.h"
#include "TutorialTarget.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>
#include <GameApp\Explosion.h>

TutorialTarget::TutorialTarget()
	: renderer_(nullptr)
	, bodyRenderer_(nullptr)
{

}

TutorialTarget::~TutorialTarget()
{

}

void TutorialTarget::Start()
{
	transform_->SetLocation(3524.f, -402.f, 0.f);
	SetHP(10);

	GameEngineTransformComponent* subTransform = CreateTransformComponent<GameEngineTransformComponent>();
	subTransform->SetLocation(0.0f, 120.f, -0.02f);
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(subTransform);
	renderer_->CreateAnimationFolder("TutorialTarget", 0.0416f);
	renderer_->SetPivot(eImagePivot::CENTER);
	renderer_->ChangeAnimation("TutorialTarget");

	bodyRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	bodyRenderer_->SetTexture("tutorial_pyramid_topper.png", true);
	bodyRenderer_->SetLocationZ(1.001f);

	pushHitEffectRenderer(renderer_);
	pushHitEffectRenderer(bodyRenderer_);

	GameEngineCollision* c = CreateTransformComponent<GameEngineCollision>(renderer_);
	c->SetCollisionType(eCollisionType::Rect);
	c->SetCollisionGroup(eCollisionGroup::MonsterHitBox);
}

void TutorialTarget::Update(float _deltaTime)
{
	MonsterBase::Update(_deltaTime);
}

void TutorialTarget::OnHit()
{
	MonsterBase::OnHit();

	if (hp_ < 1)
	{
		Explosion* explosion = level_->CreateActor<Explosion>();
		explosion->GetTransform()->SetLocation(transform_->GetWorldLocation() + float4(0.0f, -150.f, 0.0f));
		GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_player_plane_shmup_bomb_explode_01.wav");
		Release();
	}
}
