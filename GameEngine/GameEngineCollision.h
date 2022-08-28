#pragma once

#include <GameEngine\GameEngineTransformComponent.h>
#include <GameEngine\eCollisionType.h>

#include <functional>

#include <DirectXCollision.h>
#include <DirectXCollision.inl>

class GameEngineCore;
class GameEngineLevel;
class GameEngineCollision : public GameEngineTransformComponent
{
	friend GameEngineCore;
	friend GameEngineLevel;

private:
	static bool DisplayDebugRect;

public:
	GameEngineCollision();
	~GameEngineCollision();

	GameEngineCollision(const GameEngineCollision& _other) = delete;
	GameEngineCollision(GameEngineCollision&& _other) = delete;

	GameEngineCollision& operator=(const GameEngineCollision& _other) = delete;
	GameEngineCollision& operator=(const GameEngineCollision&& _other) = delete;

public:
	virtual void Start();
	virtual void Update(float _deltaTime);

public:
	template<typename T>
	GameEngineCollision* IsCollideOne(T _group) { return IsCollideOne(static_cast<int>(_group)); }

	GameEngineCollision* IsCollideOne(int _group);
	std::list<GameEngineCollision*> IsCollide(int _group);

	template<typename groupType>
	void SetCollisionGroup(groupType _group);
	void SetCollisionGroup(int _group);
	void SetCollisionType(eCollisionType _collisionType);

	int GetCollisionGroup() { return collisionGroup_; }
private:
	static void init();
	static bool CollisionCheckRectToRect(GameEngineCollision& _lhs, GameEngineCollision& _rhs);

protected:
	int collisionGroup_;
	eCollisionType collisionType_;
	DirectX::BoundingBox boundingBox_;
	DirectX::BoundingSphere sphere_;

private:
	static std::function<bool(GameEngineCollision&, GameEngineCollision&)>
		collisionCheckFunction_[static_cast<int>(eCollisionType::MAX)][static_cast<int>(eCollisionType::MAX)];
};

template<typename groupType>
inline void GameEngineCollision::SetCollisionGroup(groupType _group)
{
	SetCollisionGroup(static_cast<int>(_group));
}
