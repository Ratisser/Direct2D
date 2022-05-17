#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class Hourglass : public GameEngineActor
{
public:
	Hourglass(); 
	~Hourglass();

	Hourglass(const Hourglass& _other) = delete; 
	Hourglass(Hourglass&& _other) = delete; 

	Hourglass& operator=(const Hourglass& _other) = delete;
	Hourglass& operator=(const Hourglass&& _other) = delete;

public:
	// GameEngineActor��(��) ���� ��ӵ�
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineImageRenderer* renderer_;
};

