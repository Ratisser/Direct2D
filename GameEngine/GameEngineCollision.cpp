#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"

std::function<bool(GameEngineCollision&, GameEngineCollision&)>
GameEngineCollision::collisionCheckFunction_[static_cast<int>(eCollisionType::MAX)][static_cast<int>(eCollisionType::MAX)];

GameEngineCollision::GameEngineCollision()
	: collisionGroup_(0)
	, collisionType_(eCollisionType::MAX)
{

}

GameEngineCollision::~GameEngineCollision()
{

}

bool GameEngineCollision::CollisionCheckRectToRect(GameEngineCollision& _lhs, GameEngineCollision& _rhs)
{
	_lhs.boundingBox_.Center.z = 0;
	_rhs.boundingBox_.Center.z = 0;

	return _lhs.boundingBox_.Intersects(_rhs.boundingBox_);
}

void GameEngineCollision::Start()
{
	GameEngineTransformComponent::Start();

	level_->pushCollision(collisionGroup_, this);
}

void GameEngineCollision::Update(float _deltaTime)
{
	GameEngineTransformComponent::Update(_deltaTime);

	boundingBox_.Center = GetWorldLocation().DxXmfloat3;
	boundingBox_.Extents = GetWorldScale().halffloat4().DxXmfloat3;
}

GameEngineCollision* GameEngineCollision::IsCollideOne(int _group)
{
	std::list<GameEngineCollision*> collisionGroup = level_->GetCollisionGroup(_group);


	for (GameEngineCollision* collision : collisionGroup)
	{
		if (collision->IsUpdate() == false)
		{
			continue;
		}

		std::function func = collisionCheckFunction_[static_cast<int>(collisionType_)][static_cast<int>(collision->collisionType_)];
		if (nullptr == func)
		{
			GameEngineDebug::MsgBoxError("구현되지 않은 충돌 함수를 사용하려 했습니다.");
		}
		else
		{
			if (func(*this, *collision))
			{
				return collision;
			}
		}
	}

	return nullptr;
}

std::list<GameEngineCollision*> GameEngineCollision::IsCollide(int _group)
{
	std::list<GameEngineCollision*> collisionGroup = level_->GetCollisionGroup(_group);
	std::list<GameEngineCollision*> resultCollisionList;

	for (GameEngineCollision* collision : collisionGroup)
	{
		if (collision->IsUpdate() == false)
		{
			continue;
		}

		std::function func = collisionCheckFunction_[static_cast<int>(collisionType_)][static_cast<int>(collision->collisionType_)];
		if (nullptr == func)
		{
			GameEngineDebug::MsgBoxError("구현되지 않은 충돌 함수를 사용하려 했습니다.");
		}
		else
		{
			if (func(*this, *collision))
			{
				resultCollisionList.push_back(collision);
			}
		}
	}

	return resultCollisionList;
}

void GameEngineCollision::SetCollisionGroup(int _group)
{
	level_->popCollision(collisionGroup_, this);
	collisionGroup_ = _group;
	level_->pushCollision(_group, this);
}

void GameEngineCollision::SetCollisionType(eCollisionType _collisionType)
{
	collisionType_ = _collisionType;
}

void GameEngineCollision::init()
{
	for (int i = 0; i < static_cast<int>(eCollisionType::MAX); i++)
	{
		for (int j = 0; j < static_cast<int>(eCollisionType::MAX); j++)
		{
			collisionCheckFunction_[i][j] = nullptr;
		}
	}

	collisionCheckFunction_[static_cast<int>(eCollisionType::Rect)][static_cast<int>(eCollisionType::Rect)]
		= std::bind(GameEngineCollision::CollisionCheckRectToRect, std::placeholders::_1, std::placeholders::_2);
}
