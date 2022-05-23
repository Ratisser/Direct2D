#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineCollision;
class GameEngineImageRenderer;
class MonsterBase : public GameEngineActor
{
public:
	MonsterBase(); 
	~MonsterBase();

	MonsterBase(const MonsterBase& _other) = delete; 
	MonsterBase(MonsterBase&& _other) = delete; 

	MonsterBase& operator=(const MonsterBase& _other) = delete;
	MonsterBase& operator=(const MonsterBase&& _other) = delete;
public:
	virtual void Update(float _deltaTime);
	virtual void OnHit();

public:
	void SetHP(int _hp);
	void SubtractHP(int _damage);

	void SetHitEffectColor(const float4& _hitEffectColor) { hitEffectColor_ = _hitEffectColor; };

	int GetHP() const { return hp_; }

protected:
	void pushHitEffectRenderer(GameEngineImageRenderer* _renderer);

protected:
	const float HIT_EFFECT_TIME = 0.034f;

protected:
	int hp_;
	float hitEffectTime_;
	float4 hitEffectColor_;

private:
	std::vector<GameEngineImageRenderer*> onHitEffectedRenderers_;
};

