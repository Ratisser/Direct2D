#include "PreCompile.h"
#include "DevilChip.h"

#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineLevel.h>

#include "DevilChipPiece.h"

bool DevilChip::bFalling_ = false;

DevilChip::DevilChip()
	: bReverse_(false)
	, renderer_(nullptr)
	, collision_(nullptr)
{
}

DevilChip::~DevilChip()
{

}

void DevilChip::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	renderer_->CreateAnimationFolder("PokerChip", 0.034f, false);
	renderer_->CreateAnimationFolderReverse("PokerChipReverse", "PokerChip", 0.034f, false);
	renderer_->ChangeAnimation("PokerChip");
	renderer_->SetLocationZ(0.01f);
	bReverse_ = false;

	GameEngineTransformComponent* tc = CreateTransformComponent<GameEngineTransformComponent>();
	tc->SetLocationY(20.f);
	tc->SetLocationZ(-0.01f);

	GameEngineImageRenderer* effect = CreateTransformComponent<GameEngineImageRenderer>(tc);
	effect->CreateAnimationFolder("BurningChip");
	effect->ChangeAnimation("BurningChip");


	collision_ = CreateTransformComponent<GameEngineCollision>(renderer_);
	collision_->SetCollisionType(eCollisionType::Rect);
	collision_->SetCollisionGroup(eCollisionGroup::MonsterProjectile);
}

void DevilChip::Update(float _deltaTime)
{
	transform_->AddLocation(0.0f, -MOVE_SPEED * _deltaTime);

	if (renderer_->GetCurrentAnimation()->IsEnd_)
	{
		if (bReverse_)
		{
			renderer_->ChangeAnimation("PokerChip");
		}
		else
		{
			renderer_->ChangeAnimation("PokerChipReverse");
		}

		bReverse_ = !bReverse_;
	}

	if (nullptr != collision_->IsCollideOne(eCollisionGroup::Platform))
	{
		DevilChipPiece* cp = level_->CreateActor<DevilChipPiece>();
		cp->GetTransform()->SetLocation(transform_->GetWorldLocation());
		FallEnd();
		Release();
	}
}

void DevilChip::FallStart()
{
	bFalling_ = true;
}

void DevilChip::FallEnd()
{
	bFalling_ = false;
}

bool DevilChip::IsFalling()
{
	return bFalling_;
}
