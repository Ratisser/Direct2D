#pragma once

#include <GameEngine\GameEngineActor.h>
#include <GameEngineBase\GameEngineFSM.h>

class GameEngineImageRenderer;
class GameEngineCollision;
class MovePoint : public GameEngineActor
{
public:
	MovePoint(); 
	~MovePoint();

	MovePoint(const MovePoint& _other) = delete; 
	MovePoint(MovePoint&& _other) = delete; 

	MovePoint& operator=(const MovePoint& _other) = delete;
	MovePoint& operator=(const MovePoint&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void SetDestination(const std::string& _levelName);

private:
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startActivate(float _deltaTime);
	void updateActivate(float _deltaTime);

private:
	GameEngineFSM state_;
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;

	std::string levelName_;
};

