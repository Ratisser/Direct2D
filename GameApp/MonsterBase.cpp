#include "PreCompile.h"
#include "MonsterBase.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngine\GameEngineRenderer.h>

#include "eCollisionGroup.h"

MonsterBase::MonsterBase()
	: hp_(0)
{

}

MonsterBase::~MonsterBase()
{

}

void MonsterBase::SetHP(int _hp)
{
	hp_ = _hp;
}

void MonsterBase::SubtractHP(int _damage)
{
	hp_ -= _damage;
}
