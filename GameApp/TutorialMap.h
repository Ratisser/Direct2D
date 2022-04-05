#pragma once

#include "Map.h"

class GameEngineCollision;
class TutorialMap : public Map
{
public:
	TutorialMap(); 
	~TutorialMap();

	TutorialMap(const TutorialMap& _other) = delete; 
	TutorialMap(TutorialMap&& _other) = delete; 

	TutorialMap& operator=(const TutorialMap& _other) = delete;
	TutorialMap& operator=(const TutorialMap&& _other) = delete;

	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

private:
	GameEngineCollision* exitDoor_;
};

