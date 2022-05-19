#pragma once

#include <GameEngine\GameEngineLevel.h>

class Player;
class Flower;
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
	// GameEngineLevel을(를) 통해 상속됨
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;

public:
	Player* GetPlayer();
	Flower* GetFlower();

private:
	Player* player_;
	Flower* flower_;


	std::unique_ptr<GameEngineSoundPlayer> bgmPlayer_;
};

