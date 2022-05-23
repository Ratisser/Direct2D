#include "PreCompile.h"
#include "MonsterBase.h"

#include <GameEngine\GameEngineCollision.h>
#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngine\GameEngineRenderer.h>

#include "eCollisionGroup.h"
#include <GameEngine\GameEngineImageRenderer.h>

MonsterBase::MonsterBase()
	: hp_(0)
	, hitEffectTime_(0.0f)
	, hitEffectColor_(0.1f, 0.2f, 0.3f)
{

}

MonsterBase::~MonsterBase()
{

}

void MonsterBase::Update(float _deltaTime)
{
	hitEffectTime_ -= _deltaTime;
	if (hitEffectTime_ > 0.0f)
	{
		hitEffectTime_ -= _deltaTime;
	}
	else
	{
		for (GameEngineImageRenderer* renderer : onHitEffectedRenderers_)
		{
			renderer->SetAddColor(float4::ZERO);
		}
	}
}

void MonsterBase::OnHit()
{
	for (GameEngineImageRenderer* renderer : onHitEffectedRenderers_)
	{
		renderer->SetAddColor(hitEffectColor_);
	}

	hitEffectTime_ = HIT_EFFECT_TIME;
}

void MonsterBase::SetHP(int _hp)
{
	hp_ = _hp;
}

void MonsterBase::SubtractHP(int _damage)
{
	hp_ -= _damage;
}

void MonsterBase::pushHitEffectRenderer(GameEngineImageRenderer* _renderer)
{
	onHitEffectedRenderers_.push_back(_renderer);
}
