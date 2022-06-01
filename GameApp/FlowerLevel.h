#pragma once

#include <GameEngine\GameEngineLevel.h>

class Player;
class Flower;
class FlowerPlatform;
class FlowerLevel : public GameEngineLevel
{
public:
	FlowerLevel(); 
	~FlowerLevel();

	FlowerLevel(const FlowerLevel& _other) = delete; 
	FlowerLevel(FlowerLevel&& _other) = delete; 

	FlowerLevel& operator=(const FlowerLevel& _other) = delete;
	FlowerLevel& operator=(const FlowerLevel&& _other) = delete;

public:
	// GameEngineLevel��(��) ���� ��ӵ�
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;

public:
	Player* GetPlayer();
	Flower* GetFlower();

	void EnterPhase2();

private:
	Player* player_;
	Flower* flower_;
	
	std::vector<FlowerPlatform*> platforms_;
	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

