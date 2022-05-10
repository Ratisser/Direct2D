#include "PreCompile.h"
#include "BombBat.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineImageRenderer.h>

BombBat::BombBat()
	: renderer_(nullptr)
{

}

BombBat::~BombBat()
{

}

void BombBat::Start()
{
	ParryObjectBase::Start();
	collision_->SetCollisionGroup(eCollisionGroup::ParryMonster);
	collision_->SetScale(50.f);


}

void BombBat::Update(float _deltaTime)
{
}

void BombBat::Initialize(bool _bLeft)
{
}

void BombBat::startSummon(float _deltaTime)
{
	GameEngineSoundManager::GetInstance().PlaySoundByName("sfx_level_devil_head_devil_bomb_appear.wav");
}

void BombBat::updateSummon(float _deltaTime)
{
}

void BombBat::startFly(float _deltaTime)
{
}

void BombBat::updateFly(float _deltaTime)
{
}

void BombBat::startExplosion(float _deltaTime)
{
}

void BombBat::updateExplosion(float _deltaTime)
{
}
